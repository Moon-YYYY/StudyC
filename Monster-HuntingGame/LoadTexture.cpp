#include"LoadTexture.h"

Change Cg;
unordered_map<int, vector<SpriteConfig>> uiconfigs = {
	{0, {
		{"��Ļ",
		"picture/��Ļ.png",
		Vector2f{820, 470},
		Vector2f{1.042f,1.042f}
		}
	}
	},
	{ 1, {
	{"����","picture/DaTing.png",Vector2f{0, 0},Vector2f{0.586f,0.586f}},
	{"��ʼ��Ϸ","picture/��ʼ��Ϸ��ť.png", Vector2f{1200, 250}, Vector2f{0.5f, 0.5f}},
	{"����", "picture/���ð�ť.png", Vector2f{1200, 350}, Vector2f{0.5, 0.5}},
	{"����", "picture/������ť.png", Vector2f{1200, 450}, Vector2f{0.5, 0.5}},
	{"�˳���Ϸ", "picture/�˳���Ϸ��ť.png", Vector2f{1200, 550}, Vector2f{0.5, 0.5}}
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
		ts1 = getSprite("��ʼ��Ϸ");
		ts2 = getSprite("����");
		ts3 = getSprite("����");
		ts4 = getSprite("�˳���Ϸ");
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
		//��ʼ��Ϸ��ť��Ч
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

	//���ð�ť��Ч
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

	//������ť��Ч
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

	//�˳���Ϸ��ť
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
//�ͷŷǵ�ǰ������Դ
void LoadTexture::releaseNonCurrIDResources(int currentID) {
	vector<int> keysToRemove;
	for (auto& pair : textures) {
		if (pair.first != currentID) {
			keysToRemove.push_back(pair.first);
		}
	}
	for (int key : keysToRemove) {
		cout << "�ͷŽ���" << key << "����Դ" << endl;
		textures.erase(key);
	}

}
//����ָ���������Դ��ѧϰ����Ŀ�ĵ�n�졪������
void LoadTexture::LoadUIResources(int uiID) {
	cout << "����" << uiID << "����Դ" << endl;
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
		cout << "δ֪����" << endl;
	}
	if (!sprites.empty()) {
		textures[uiID] = sprites;
	}
}
//�������飬ΪʲôҪ�о����ⶫ���������Լ���������ȥ���Ǻ�����
TextureSprite LoadTexture::createSprite(const string& path) {
	TextureSprite ts;
	ts.texture = make_shared<Texture>();
	if (!ts.texture->loadFromFile(path)) {
		cout << "����ʧ��" << path << endl;
		ts.sprite = make_shared<Sprite>();
		return ts;
	}
	ts.sprite = make_shared<Sprite>();
	ts.sprite->setTexture(*ts.texture);
	return ts;
}