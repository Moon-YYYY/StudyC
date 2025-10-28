#pragma once
#include<SFML/Graphics.hpp>
#include<vector>
#include<memory>
#include<unordered_map>
#include<string>
#include<iostream>
#include"People.h"
using namespace sf;
using namespace std;
struct TextureSprite {//��������ָ���Զ������ڴ棬��ͦ����~
	string name;
	shared_ptr<Sprite>sprite;
	shared_ptr<Texture>texture;
};
struct SpriteConfig {
	string name;
	string path;
	Vector2f PicturePos;
	Vector2f scale;
};

struct Change {
	Vector2f KaiMu_Scale = { 1.1f, 1.1f };
	Vector2f DaTingButton_Add = { 0.5f, 0.5f };
	Vector2f DaTingButton_remove = { 0.5f, 0.5f };
	float speed = 0.01;
	float ButtonSpeed_Add = 0;
	float ButtonSpeed_Remove = 0;
}Cg;
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
	{"��ʼ��Ϸ","picture/��ʼ��Ϸ��ť.png", Vector2f{1200, 250}, Vector2f{0.5f, 0.5f}}
	}}
};
class LoadTexture {
public:
	void setCurrentUI(int uiID) {
		if (currentID != uiID) {
			releaseNonCurrIDResources(uiID);
			currentID = uiID;
		}
		if (textures.find(uiID) == textures.end()) {
			LoadUIResources(uiID);
		}
	
	}
	TextureSprite KaiMu_change(TextureSprite& ts){
		if (ts.sprite->getOrigin() == Vector2f(0, 0)) {
			ts.sprite->setOrigin(800, 450);
		}
		if (Cg.speed < 0.65) {
			Cg.speed += 0.0007;
			ts.sprite->setScale(Cg.KaiMu_Scale.x + Cg.speed, Cg.KaiMu_Scale.y + Cg.speed);
		}
		else if(Cg.speed >= 0.65 && Cg.speed <= 12){
			Cg.speed += 0.1;
			ts.sprite->setScale(Cg.KaiMu_Scale.x + Cg.speed, Cg.KaiMu_Scale.y + Cg.speed);
		}
		else if(Cg.speed > 12){
			ST.UI = 1;
		}
		return ts;
	}
	void DaTingButtonUpdata(Vector2i& MousePosition) {/*
		cout << MousePosition.x << ' ' << MousePosition.y << endl;*/
		TextureSprite* ts1 = getSprite("��ʼ��Ϸ");
		if (ts1->sprite->getOrigin() == Vector2f(0,0)) {
			ts1->sprite->setOrigin(640, 80);
		}
		if (MousePosition.x > 921 && MousePosition.x < 1475 && MousePosition.y > 215 && MousePosition.y < 288 ) {
			if (Cg.ButtonSpeed_Add < 0.05) {
				Cg.ButtonSpeed_Add += 0.0001;
				ts1->sprite->setScale(Cg.DaTingButton_Add.x + Cg.ButtonSpeed_Add, Cg.DaTingButton_Add.y + Cg.ButtonSpeed_Add);
				Cg.ButtonSpeed_Remove = 0;
			}
		}
		else {
			Vector2f Scale_BeginButton = ts1->sprite->getScale();/*
			cout << Scale_BeginButton.x << ' ' << Scale_BeginButton.y  << endl;*/
			if (Cg.ButtonSpeed_Remove < 0.05) {
				Cg.ButtonSpeed_Remove += 0.0001;
				ts1->sprite->setScale(Cg.DaTingButton_remove.x - Cg.ButtonSpeed_Remove, Cg.DaTingButton_remove.y - Cg.ButtonSpeed_Remove);
				Cg.ButtonSpeed_Add = 0;
			}
		}
	}
	void Updata(Vector2i& MousePosition) {
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
	void draw(RenderWindow& win) {
		auto it = textures.find(currentID);
		if (it != textures.end()) {
			for (auto& ts : it->second) {
				if (ts.sprite) {
					win.draw(*ts.sprite);
				}
			}
		}
	}
	TextureSprite* getSprite(const string& name) {
		auto it = textures.find(currentID);
		if (it != textures.end()) {
			for (auto& ts : it->second) {
				if (ts.name == name)
					return &ts;
			}
		}
		return nullptr;
	}
private:
	//�ͷŷǵ�ǰ������Դ
	void releaseNonCurrIDResources(int currentID) {
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
	//����ָ���������Դ������~~~ѧϰ����Ŀ�ĵ�n�졪������
	void LoadUIResources(int uiID) {
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
	TextureSprite createSprite(const string& path) {
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
private:
	int currentID = -1;
	unordered_map<int, vector<TextureSprite>>textures;
};