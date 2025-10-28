#pragma once
#include<SFML/Graphics.hpp>
#include<vector>
#include<memory>
#include<unordered_map>
#include<Windows.h>
#include<string>
#include<iostream>
#include"People.h"
#include"KeyAndMouse.h"
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
	//������ť��Ч�ٶ�
	float BeginGameScale = 0.5;
	float SettingButton = 0.5;
	float HelpButton = 0.5;
	float ExitButton = 0.5;
	float ScaleChangeSpeed = 0.0005;
	float speed = 0.01;
	float ButtonSpeed_Add = 0;
	float ButtonSpeed_Remove = 0;
};
extern Change Cg;
extern unordered_map<int, vector<SpriteConfig>> uiconfigs;
class LoadTexture {
public:
	void setCurrentUI(int uiID);
	TextureSprite KaiMu_change(TextureSprite& ts);
	void LoadButtonResources();
	void DaTingButtonUpdata(Vector2i& MousePosition, Event& event);
	void Updata(Vector2i& MousePosition, Event& event);
	void draw(RenderWindow& win);
	TextureSprite* getSprite(const string& name);
private:
	//�ͷŷǵ�ǰ������Դ
	void releaseNonCurrIDResources(int currentID);
	//����ָ���������Դ��ѧϰ����Ŀ�ĵ�n�졪������
	void LoadUIResources(int uiID);
	//�������飬ΪʲôҪ�о����ⶫ���������Լ���������ȥ���Ǻ�����
	TextureSprite createSprite(const string& path);
private:
	int currentID = -1;
	TextureSprite* ts1 = nullptr;
	TextureSprite* ts2 = nullptr;
	TextureSprite* ts3 = nullptr;
	TextureSprite* ts4 = nullptr;
	bool isLoadButton = false;
	unordered_map<int, vector<TextureSprite>>textures;
};