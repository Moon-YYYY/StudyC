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
struct TextureSprite {//设置智能指针自动管理内存，还挺好用~
	shared_ptr<Sprite>sprite;
	shared_ptr<Texture>texture;
};
struct SpriteConfig {
	string path;
	Vector2f PicturePos;
	Vector2f scale;
};

struct Change {
	Vector2f KaiMu_Scale = { 1.1f, 1.1f };
	float speed = 0.01;
}Cg;
unordered_map<int, vector<SpriteConfig>> uiconfigs = {
	{0, {
		{
		"picture/开幕.png",
		Vector2f{820, 470},
		Vector2f{1.042f,1.042f}
		}
	}
	},
	{ 1, {
	{"picture/DaTing.png",Vector2f{0, 0},Vector2f{0.586f,0.586f}}
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
	TextureSprite change(TextureSprite& ts){
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
	void Updata() {
		auto it = textures.find(currentID);
		if (it != textures.end()) {
			for (auto& ts : it->second) {
				change(ts);
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
private:
	//释放非当前界面资源
	void releaseNonCurrIDResources(int currentID) {
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
	//加载指定界面的资源，好难~~~学习做项目的第n天——于洋
	void LoadUIResources(int uiID) {
		cout << "加载" << uiID << "的资源" << endl;
		vector<TextureSprite> sprites;
		auto configIt = uiconfigs.find(uiID);
		if (configIt != uiconfigs.end()) {
			for (auto& config : configIt->second) {
				TextureSprite ts = createSprite(config.path);
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
	TextureSprite createSprite(const string& path) {
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
private:
	int currentID = -1;
	unordered_map<int, vector<TextureSprite>>textures;
};