# include <Siv3D.hpp> // OpenSiv3D v0.6.5

// 取得した座標を中心とする半径50くらい？の円を考える．




void Main()
{

	//define shape
	constexpr Triangle shape{ 0,0,0,400,600,400 };

	//円
	Circle circle{};

	Vec2 point = { 0,0 };

	int highscore = 0;

	//texture unko
	const Texture unko{ U"💩"_emoji };

	int initial_x = 300;
	int initial_y = 200;

	point = { initial_x,initial_y };

	Vec2 center = { initial_x,initial_y };

	circle = { center,50 };

	int score = 0;

	RectF rectF{ 0,0,0,0 };
	Rect replay{ 500,510,200,35 };

	Stopwatch stopwatch{ StartImmediately::Yes };

	const Font font{ FontMethod::MSDF, 48 };

	Microphone mic{ StartImmediately::Yes };

	if (not mic) throw Error{ U"Microphone not available" };

	FFTResult fft;

	bool f = 0;
	bool Flag = false;

	String sceneName = U"GameScene";


	while (System::Update())
	{
		if (sceneName == U"StartScene") {
			// StartSceneの時の処理

			String text = fmt::format(U"あなたのスコアは{}でした！", score);
		

			stopwatch.reset();
			if (score > highscore) {
				highscore = score;
				Flag = true;
			}
			if (Flag) {
				text += U" highest更新！";
			}

			text += U" #スペクトルバスターre";

		

			

			
			

			font(U"スコア　:　{}"_fmt(score)).draw(70, Vec2{ 20,10 }, Palette::White);
			font(U"ハイスコア　:　{}"_fmt(highscore)).draw(30, Vec2{ 300,400 }, Palette::White);
			font(U"もう一度遊ぶ").draw(30, Vec2{ 500,500 }, Palette::White);
			//replay.draw();
			if (replay.leftClicked()) {
				sceneName = U"GameScene";
				score = 0;
				Flag = false;
			}


			if (SimpleGUI::Button(U"Tweet", Vec2{ 20, 400 }))
			{
				// text をつぶやくツイート投稿画面を開く
				Twitter::OpenTweetWindow(text);
			}

		}
		else if (sceneName == U"GameScene") {
			// GameSceneの時の処理

			stopwatch.start();

			mic.fft(fft);

			for (auto i : step(800)) {
				f = 0;
				const double size = Pow(fft.buffer[i], 0.6f) * 10000;
				rectF = { Arg::bottomLeft(i,600),1,size };
				rectF.draw(HSV{ 240 - i });
				if (circle.intersects(rectF) && !f) {
					point = RandomVec2(shape);
					circle.center = point;
					center = point;
					score++;
					f = 1;
				}


			}

			font(U"スコア　:　{}"_fmt(score)).draw(30, Vec2{ 10,10 }, Palette::White);
			font(U"残り時間　: {}"_fmt(30 - stopwatch.s())).draw(50, Vec2{ 10,70 }, Palette::White);
			unko.drawAt(point);
			//circle.draw();

			if (stopwatch.s() >= 30) {
				sceneName = U"StartScene";
			}

		}
	}
}

