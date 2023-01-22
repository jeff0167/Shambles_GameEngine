#include "../Shambles_GameEngine/Shambles.h" // all necessary files are within shambles.h

// everything main includes, needs to be included here, 
// could you perhaps make a bundle header that holds all needed files at once to then only having to use one include

using namespace Shambles;
using namespace sf;
using namespace std;

Application app;

SpriteRenderer player, Rnotplayer, background;

Texture particle;

float moveSpeed = 50.0f; // MoveSpeed was 5.0f as default
Vector2f velocity;

void Die();
void ResetStones();
void SetScore();
void CreateStone();
void ReStartGame();
void MakeStones();
void MakePlayer();
void KeyboardInput();
void PlayerAnimState();
void CollisionChecking();
void PlayerCollision(Collider* other);

unique_ptr<BoxCollider> collider;
unique_ptr<ParticleSystem> ps;
unique_ptr<GameObject> g, gNotPlayer;
unique_ptr<Rigidbody> rb;

Animation _playerAnim;
Texture hero, stoneTexture, backgroundTexture;

UI_Text scoreText, highScoreText, scoreTextValue, highScoreTextValue, youDiedText;

bool idle = true;

vector<GameObject> Stones;

int ScoreValue = 0, highScoreValue;
Vector2f startPos(960, 1020);

Rigidbody rbToChange;

//View camera; // usable if you need to move, scale and rotate the view, like any camera in Unity

int main()
{
	sf::Color color(173, 96, 0);
	sf::Color deadColor(161, 3, 0);
	int charSize = 55;

	scoreText = UI_Text(Vector2f(25, 5), "Score: ", charSize, color);
	Renderer.AddDrawable(scoreText);
	Renderer.ChangeDrawableLayer(scoreText, 9);
	scoreTextValue = UI_Text(Vector2f(145, 5), "0", charSize, color);
	Renderer.AddDrawable(scoreTextValue);
	Renderer.ChangeDrawableLayer(scoreTextValue, 9);
	
	highScoreText = UI_Text(Vector2f(1600, 5), "High Score: ", charSize, color);
	Renderer.AddDrawable(highScoreText);
	Renderer.ChangeDrawableLayer(highScoreText, 9);
	highScoreTextValue = UI_Text(Vector2f(1805, 5), "0", charSize, color);
	Renderer.AddDrawable(highScoreTextValue);
	Renderer.ChangeDrawableLayer(highScoreTextValue, 9);

	youDiedText = UI_Text(Vector2f(700, 2040), "You died", charSize * 3.5f, deadColor);
	Renderer.AddDrawable(youDiedText);
	Renderer.ChangeDrawableLayer(youDiedText, 9);

	float multi = 1.39f;
	backgroundTexture.loadFromFile("StoneCave.png");
	background = SpriteRenderer(backgroundTexture, Vector2f(1400 * multi, 787 * multi), Vector2f(960, 540), 0); // 1920/2 and 1080/2
	Renderer.AddDrawable(background);

	app.AddAwakeFunctions(MakePlayer);
	app.AddAwakeFunctions(MakeStones);

	app.AddUpdateFunctions(ResetStones);
	app.AddUpdateFunctions(KeyboardInput);
	app.AddUpdateFunctions(PlayerAnimState);
	app.AddUpdateFunctions(CollisionChecking);

	Mono.Invoke(SetScore, 1000);
	app.Start();
	DebugLog("Hello Universe!");
	return 0;
}

void MakePlayer()
{
	player = SpriteRenderer("_sprites_heroes.png", Vector2f(100, 200), Vector2f(960, 1020));
	_playerAnim = Animation(player.getTexture(), Vector2u(9, 8), 0.15f);

	Vector2u textureSize = player.getTexture()->getSize(); // 9 * 8
	textureSize.x /= 9;
	textureSize.y /= 8;

	player.setOrigin(textureSize.x * (float)3.5, player.getTexture()->getSize().y / 2); // check what these values give,  the character in the image are not centered in their so called box, need app better image to do this with
	player.setTextureRect(IntRect(textureSize.x * 1, textureSize.y * 7, textureSize.x, textureSize.y));

	collider = unique_ptr<BoxCollider>(new BoxCollider(player, player.getPosition()));
	collider.get()->rect->setOrigin(player.getOrigin().x / 2, player.getOrigin().y / 2);
	collider.get()->OnCollision = &PlayerCollision;

	rb = unique_ptr<Rigidbody>(new Rigidbody());
	ps = unique_ptr<ParticleSystem>(new ParticleSystem(&player, 100, 10, 50, seconds(1), Color::Green));  // both goes out of scope, !WORKS NOW
	g = unique_ptr<GameObject>(new GameObject(player, *ps.get(), *rb.get(), *collider.get())); // but I add gameObject to the scene? and the gameObject has the ref of the particleSystem

	Renderer.ChangeDrawableLayer(player, 9);
}

void PlayerCollision(Collider* other)
{
	Die();
}

void MakeStones()
{
	Science.SetGravity(Vector2f(0, 1.5f)); // lowering gravity
	stoneTexture.loadFromFile("Stone.jpg");

	for (size_t i = 0; i < 25; i++) // boxcollider gets destroyed double the amount of all the ohter components
	{
		Rigidbody* rb = new Rigidbody();
		SpriteRenderer* sprite = new SpriteRenderer(stoneTexture, Vector2f(50, 50), Vector2f(0, 2000));
		BoxCollider* collider = new BoxCollider(*sprite, sprite->getPosition());
		collider->rect->setOrigin(Vector2f(collider->rect->getSize().x / 2, collider->rect->getSize().y / 2));
		ParticleSystem* ps = new ParticleSystem(collider->shape, 20, 7, 10, seconds(0.5f));
		ps->SetTexture(&stoneTexture);

		GameObject* g = new GameObject(*sprite, *rb, *collider, *ps);
		rb->useGravity = true;

		Stones.emplace_back(*g);
	}
	ResetStones();
}

void ResetStones()
{
	for (auto& stone : Stones)
	{
		if (stone.transform->getPosition().y > 1080)
		{
			float posOffsetX = (rand() % 1920) - 100; // now that I think of it, the rand is not very intuitive and is confusing to use, I should make my own version
			float posOffsetY = (rand() % 100) - 500;
			stone.transform->setPosition(Vector2f(540 + posOffsetX, -100 + posOffsetY));
			float speed = (rand() % 100) - 50;
			float pullSpeed = (rand() % 10) -5.0f;
			//DebugLog(speed);
			stone.GetComponent(Rigidbody())->velocity = Vector2f(speed, pullSpeed);
		}
	}
}

void SetScore()
{
	ScoreValue++;
	scoreTextValue.setString(to_string(ScoreValue));
	Mono.Invoke(SetScore, 1000); // quite insecure about this method of doing it, but seems to work alright for now
}

void Die()
{
	youDiedText.setPosition(Vector2f(700, 240)); // didn't simply let me remove it from the renderer list without causing errors
	player.setPosition(600, -200);
	if (ScoreValue > highScoreValue)
	{
		highScoreValue = ScoreValue;
		highScoreTextValue.setString(to_string(highScoreValue));
	}
	Mono.Invoke(ReStartGame, 2500);
}

void ReStartGame()
{
	ScoreValue = 0;
	player.setPosition(startPos);
	youDiedText.setPosition(Vector2f(700, 2040));
}

void KeyboardInput()
{
	velocity = Vector2f(0, 0);
	if (Input::GetKey(Keyboard::Escape, Input::KeyDown)) window.close();

	//if (Input::GetKey(Keyboard::Space, Input::KeyDown)) Shoot();

	if (Input::GetKey(Keyboard::Left, Input::KeyHeld) || Input::GetKey(Keyboard::A, Input::KeyHeld))
	{
		velocity.x = -1;
	}
	if (Input::GetKey(Keyboard::Right, Input::KeyHeld) || Input::GetKey(Keyboard::D, Input::KeyHeld))
	{
		velocity.x = 1;
	}
	//if (Input::GetKey(Keyboard::Up, Input::KeyHeld) || Input::GetKey(Keyboard::W, Input::KeyHeld))
	//{
	//	velocity.y = -1;
	//}
	//if (Input::GetKey(Keyboard::Down, Input::KeyHeld) || Input::GetKey(Keyboard::S, Input::KeyHeld))
	//{
	//	velocity.y = 1;
	//}
	rb.get()->velocity = Mathf::Normalize(velocity) * moveSpeed;
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

	//if (window.getSize().y < player.getPosition().y + player.getLocalBounds().height / 8) // beware the 8
	//	player.setPosition(player.getPosition().x, window.getSize().y - player.getLocalBounds().height / 8); // checking for window y

	//if (0 > player.getPosition().y - player.getLocalBounds().height / 2)
	//	player.setPosition(player.getPosition().x, 0 + player.getLocalBounds().height / 2);
}