#pragma once

#include "ofMain.h"

#include "ofxMidi.h"

class testApp : public ofBaseApp, public ofxMidiListener {

public:

	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);

	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();

	void newMidiMessage(ofxMidiMessage& eventArgs);

	void drawKeyboard();
    void drawPotentiometer(float radAngle);
	bool m_bDrawKeyboard;
	int m_iNumOctaves;
	int m_iKeyOffset;

	stringstream m_sText;
	string m_sProgram;

	ofxMidiIn midiIn;
	ofxMidiMessage midiMessage;

	vector<int> m_vecKeys;
	int m_iPitch; // 0--16384
	int m_iMod; // 0--127
	int m_iCutOff; // 0--256
	int m_iResonance; // 0--256
	int m_iAttack; // 0--256
	int m_iRelease; // 0--256
	float m_fTempo; // 20--300
	float m_fTime; // Used to measure tempo

};
