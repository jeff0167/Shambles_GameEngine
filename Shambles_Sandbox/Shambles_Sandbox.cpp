#include "../Shambles_GameEngine/Shambles.h" // all necessary files are within shambles.h

// everything main includes, needs to be included here, 
// could you perhaps make a bundle header that holds all needed files at once to then only having to use one include

using namespace Shambles;
using namespace sf;
using namespace std;

Application app;

SpriteRenderer player;

Texture particle;

float moveSpeed = 50.0f; // MoveSpeed was 5.0f as default
Vector2f velocity;

void Shoot();
void MouseInput();
void MakePlayer();
void KeyboardInput();
void PlayerAnimState();
void CollisionChecking();

unique_ptr<ParticleSystem> ps;
unique_ptr<GameObject> g;
unique_ptr<Rigidbody> rb;

Animation _playerAnim;
Texture hero;

UI_Text someText;

bool idle = true;

//View camera; // usable if you need to move, scale and rotate the view, like any camera in Unity

int main()
{
	// I do wonder if I could make use of it in my code, just wouldn't know the cons and pros between them and what would be best for my scenario
	// emplace_back vs push_back for vector optimization? emplace avoids copying, with is good but then why ever use push_back?
	// .reserve(size) to reserve space for a vector so the vector doesn't need to copy cause of it's expected size changes/grow, also then prevents copying

	// stack overflow comment, that actually makes sense
	// Specific use case for emplace_back: If you need to create a temporary object which will then be pushed into a container
	// use emplace_back instead of push_back.It will create the object in - place within the container.
	// Notes :
	// 1. push_back in the above case will create a temporary objectand move it into the container.However
	// in - place construction used for emplace_back would be more performant than constructingand then moving the object(which generally involves some copying).
	// 2. In general, you can use emplace_back instead of push_back in all the cases without much issue.

	// implicit conversion sounds like what I need for the template collision overload func call issue, u know, though I might be wrong

	someText = UI_Text(Vector2f(600, 600), "Knowledge!", 100);
	Renderer.AddDrawable(someText);

	app.AddAwakeFunctions(MakePlayer);
	app.AddUpdateFunctions(PlayerAnimState);
	app.AddUpdateFunctions(KeyboardInput);
	app.AddUpdateFunctions(MouseInput);
	app.AddUpdateFunctions(CollisionChecking);
	app.Start();
	DebugLog("Hello Universe!");
	return 0;
}

void MakePlayer()
{
	player = SpriteRenderer("_sprites_heroes.png", Vector2f(100, 200), Vector2f(600, 600));
	_playerAnim = Animation(player.getTexture(), Vector2u(9, 8), 0.15f);

	Vector2u textureSize = player.getTexture()->getSize(); // 9 * 8
	textureSize.x /= 9;
	textureSize.y /= 8;

	player.setOrigin(textureSize.x * (float)3.5, player.getTexture()->getSize().y / 2); // check what these values give,  the character in the image are not centered in their so called box, need app better image to do this with
	player.setTextureRect(IntRect(textureSize.x * 1, textureSize.y * 7, textureSize.x, textureSize.y));

	rb = unique_ptr<Rigidbody>(new Rigidbody());
	ps = unique_ptr<ParticleSystem>(new ParticleSystem(&player, 50, 5, 50, seconds(1)));  // both goes out of scope, !WORKS NOW
	g = unique_ptr<GameObject>(new GameObject(player, *ps.get(), *rb.get())); // but I add gameObject to the scene? and the gameObject has the ref of the particleSystem
	Renderer.ChangeDrawableLayer(player, 9);
}

void KeyboardInput()
{
	velocity = Vector2f(0, 0);
	if (Input::GetKey(Keyboard::Escape, Input::KeyDown)) window.close();

	if (Input::GetKey(Keyboard::Space, Input::KeyDown)) Shoot();

	if (Input::GetKey(Keyboard::Left, Input::KeyHeld) || Input::GetKey(Keyboard::A, Input::KeyHeld))
	{
		velocity.x = -1;
	}
	if (Input::GetKey(Keyboard::Right, Input::KeyHeld) || Input::GetKey(Keyboard::D, Input::KeyHeld))
	{
		velocity.x = 1;
	}
	if (Input::GetKey(Keyboard::Up, Input::KeyHeld) || Input::GetKey(Keyboard::W, Input::KeyHeld))
	{
		velocity.y = -1;
	}
	if (Input::GetKey(Keyboard::Down, Input::KeyHeld) || Input::GetKey(Keyboard::S, Input::KeyHeld))
	{
		velocity.y = 1;
	}
	rb.get()->velocity = Mathf::Normalize(velocity) * moveSpeed;
}

void MouseInput()
{
	if (Mouse::isButtonPressed(Mouse::Left))
	{
		Vector2i mousePos = Mouse::getPosition(window);
		player.setPosition((mousePos.x), (mousePos.y));
	}
}

void PlayerAnimState()
{
	if (rb.get()->velocity.x < 0 && player.getScale().x > 0) // what way are we facing?
		player.setScale(-1, player.getScale().y);

	else if (rb.get()->velocity.x > 0 && player.getScale().x < 0)
		player.setScale(1, player.getScale().y);

	if (rb.get()->velocity != Mathf::Zero()) // we are moving
	{
		if (idle)
		{
			idle = false;
			_playerAnim.NextAnim();
		}
		_playerAnim.Update(7, 3, 6, Mono.DeltaTime);
	}
	else
	{
		if (!idle)
		{
			idle = true;
			_playerAnim.NextAnim();
		}

		_playerAnim.Update(7, 0, 3, Mono.DeltaTime);
	}
	player.setTextureRect(_playerAnim.uvRect);
}

void CollisionChecking() 
{
	if (window.getSize().x < player.getPosition().x + player.getLocalBounds().width / 2)
		player.setPosition(window.getSize().x - player.getLocalBounds().width / 2, player.getPosition().y); // checking for window x

	if (0 > player.getPosition().x - player.getLocalBounds().width / 2)
		player.setPosition(0 + player.getLocalBounds().width / 2, player.getPosition().y);

	if (window.getSize().y < player.getPosition().y + player.getLocalBounds().height / 8) // beware the 8
		player.setPosition(player.getPosition().x, window.getSize().y - player.getLocalBounds().height / 8); // checking for window y

	if (0 > player.getPosition().y - player.getLocalBounds().height / 2)
		player.setPosition(player.getPosition().x, 0 + player.getLocalBounds().height / 2);
}

void Shoot() 
{
	CircleShape* s = new CircleShape(10, 50);
	s->setPosition(player.getPosition() + Vector2f(0, 100) + Vector2f(+40, 0) + (Vector2f(70, 0) * player.getScale().x));
	s->setOrigin(player.getOrigin());
	s->setFillColor(Color::Magenta);

	Rigidbody* r = new Rigidbody();
	GameObject g = GameObject(*s, *r);
	r->useGravity = true;
	r->velocity = Vector2f(player.getScale().x * 60, 0);
}