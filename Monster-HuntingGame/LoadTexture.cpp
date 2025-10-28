#include"LoadTexture.h"

Change Cg;
unordered_map<int, vector<SpriteConfig>> uiconfigs = {
	{0, {
		{"开幕",
		"picture/开幕.png",
		Vector2f{820, 470},
		Vector2f{1.042f,1.042f}
		}
	}
	},
	{ 1, {
	{"大厅","picture/DaTing.png",Vector2f{0, 0},Vector2f{0.586f,0.586f}},
	{"开始游戏","picture/开始游戏按钮.png", Vector2f{1200, 250}, Vector2f{0.5f, 0.5f}},
	{"设置", "picture/设置按钮.png", Vector2f{1200, 350}, Vector2f{0.5, 0.5}},
	{"帮助", "picture/帮助按钮.png", Vector2f{1200, 450}, Vector2f{0.5, 0.5}},
	{"退出游戏", "picture/退出游戏按钮.png", Vector2f{1200, 550}, Vector2f{0.5, 0.5}}
	}}
};
void LoadTexture::setCurrentUI(int uiID) {
	if (currentID != uiID) {
		releaseNonCurrIDResources(uiID);
		currentID = uiID;
	}
	if (textures.find(uiID) == textures.end()) {
		LoadUIResources(uiID);
	}

}
TextureSprite LoadTexture::KaiMu_change(TextureSprite& ts) {
	if (ts.sprite->getOrigin() == Vector2f(0, 0)) {
		ts.sprite->setOrigin(800, 450);
	}
	if (Cg.speed < 0.65) {
		Cg.speed += 0.0007;
		ts.sprite->setScale(Cg.KaiMu_Scale.x + Cg.speed, Cg.KaiMu_Scale.y + Cg.speed);
	}
	else if (Cg.speed >= 0.65 && Cg.speed <= 12) {
		Cg.speed += 0.1;
		ts.sprite->setScale(Cg.KaiMu_Scale.x + Cg.speed, Cg.KaiMu_Scale.y + Cg.speed);
	}
	else if (Cg.speed > 12) {
		ST.UI = 1;
	}
	return ts;
}
void LoadTexture::LoadButtonResources() {
	if (!isLoadButton) {
		ts1 = getSprite("开始游戏");
		ts2 = getSprite("设置");
		ts3 = getSprite("帮助");
		ts4 = getSprite("退出游戏");
		isLoadButton = true;
	}
}
void LoadTexture::DaTingButtonUpdata(Vector2i& MousePosition) {
	LoadButtonResources();
		if (ts1->sprite->getOrigin() == Vector2f(0, 0)) {
			ts1->sprite->setOrigin(640, 80);
			ts2->sprite->setOrigin(640, 80);
			ts3->sprite->setOrigin(640, 80);
			ts4->sprite->setOrigin(640, 80);
		}
		//开始游戏按钮动效
	if (MousePosition.x > 921 && MousePosition.x < 1475 && MousePosition.y > 215 && MousePosition.y < 288) {
		if (Cg.BeginGameScale <= 0.6) {
			Cg.BeginGameScale += Cg.ScaleChangeSpeed;
			ts1->sprite->setScale(Cg.BeginGameScale, Cg.BeginGameScale);
		}
	}
	else {
		if (Cg.BeginGameScale >= 0.5) {
			Cg.BeginGameScale -= Cg.ScaleChangeSpeed;
			ts1->sprite->setScale(Cg.BeginGameScale, Cg.BeginGameScale);
		}
	}

	//设置按钮动效
	if (MousePosition.x > 921 && MousePosition.x < 1475 && MousePosition.y > 315 && MousePosition.y < 388) {
		if (Cg.SettingButton <= 0.6) {
			Cg.SettingButton += Cg.ScaleChangeSpeed;
			ts2->sprite->setScale(Cg.SettingButton, Cg.SettingButton);
		}
	}
	else {
		if (Cg.SettingButton >= 0.5) {
			Cg.SettingButton -= Cg.ScaleChangeSpeed;
			ts2->sprite->setScale(Cg.SettingButton, Cg.SettingButton);
		}
	}

	//帮助按钮动效
	if (MousePosition.x > 921 && MousePosition.x < 1475 && MousePosition.y > 415 && MousePosition.y < 488) {
		if (Cg.HelpButton <= 0.6) {
			Cg.HelpButton += Cg.ScaleChangeSpeed;
			ts3->sprite->setScale(Cg.HelpButton, Cg.HelpButton);
		}
	}
	else {
		if (Cg.HelpButton >= 0.5) {
			Cg.HelpButton -= Cg.ScaleChangeSpeed;
			ts3->sprite->setScale(Cg.HelpButton, Cg.HelpButton);
		}
	}

	//退出游戏按钮
	if (MousePosition.x > 921 && MousePosition.x < 1475 && MousePosition.y > 515 && MousePosition.y < 588) {
		if (Cg.ExitButton <= 0.6) {
			Cg.ExitButton += Cg.ScaleChangeSpeed;
			ts4->sprite->setScale(Cg.ExitButton, Cg.ExitButton);
		}
	}
	else {
		if (Cg.ExitButton >= 0.5) {
			Cg.ExitButton -= Cg.ScaleChangeSpeed;
			ts4->sprite->setScale(Cg.ExitButton, Cg.ExitButton);
		}
	}
}
void LoadTexture::Updata(Vector2i& MousePosition) {
	auto it = textures.find(currentID);
	if (it != textures.end()) {
		for (auto& ts : it->second) {
			switch (currentID) {
			case 0:
				KaiMu_change(ts);
				break;
			case 1:
				DaTingButtonUpdata(MousePosition);
				break;
			default:
				break;
			}

		}
	}
}
void LoadTexture::draw(RenderWindow& win) {
	auto it = textures.find(currentID);
	if (it != textures.end()) {
		for (auto& ts : it->second) {
			if (ts.sprite) {
				win.draw(*ts.sprite);
			}
		}
	}
}
TextureSprite* LoadTexture::getSprite(const string& name) {
	auto it = textures.find(currentID);
	if (it != textures.end()) {
		for (auto& ts : it->second) {
			if (ts.name == name)
				return &ts;
		}
	}
	return nullptr;
}
//释放非当前界面资源
void LoadTexture::releaseNonCurrIDResources(int currentID) {
	vector<int> keysToRemove;
	for (auto& pair : textures) {
		if (pair.first != currentID) {
			keysToRemove.push_back(pair.first);
		}
	}
	for (int key : keysToRemove) {
		cout << "释放界面" << key << "的资源" << endl;
		textures.erase(key);
	}

}
//加载指定界面的资源，学习做项目的第n天——于洋
void LoadTexture::LoadUIResources(int uiID) {
	cout << "加载" << uiID << "的资源" << endl;
	vector<TextureSprite> sprites;
	auto configIt = uiconfigs.find(uiID);
	if (configIt != uiconfigs.end()) {
		for (auto& config : configIt->second) {
			TextureSprite ts = createSprite(config.path);
			ts.name = config.name;
			if (ts.sprite) {
				ts.sprite->setPosition(config.PicturePos);
				ts.sprite->setScale(config.scale);
			}
			Vector2u xy = ts.texture->getSize();
			cout << xy.x << ' ' << xy.y << endl;
			sprites.push_back(ts);
		}
	}
	else {
		cout << "未知界面" << endl;
	}
	if (!sprites.empty()) {
		textures[uiID] = sprites;
	}
}
//创建精灵，为什么要有精灵这东西，纹理自己不会贴上去吗，那很懒了
TextureSprite LoadTexture::createSprite(const string& path) {
	TextureSprite ts;
	ts.texture = make_shared<Texture>();
	if (!ts.texture->loadFromFile(path)) {
		cout << "加载失败" << path << endl;
		ts.sprite = make_shared<Sprite>();
		return ts;
	}
	ts.sprite = make_shared<Sprite>();
	ts.sprite->setTexture(*ts.texture);
	return ts;
}