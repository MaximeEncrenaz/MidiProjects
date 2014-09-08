#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetVerticalSync(true);
	ofBackground(120, 0, 0);
	ofSetLogLevel(OF_LOG_VERBOSE);

	// print input ports to console
	midiIn.listPorts(); // via instance
	//ofxMidiIn::listPorts(); // via static as well

	// open port by number (you may need to change this)
	midiIn.openPort(1);
	//midiIn.openPort("IAC Pure Data In");	// by name
	//midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port

	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	midiIn.ignoreTypes(false, true, false);

	//midiIn.setVerbose(true);

	// add testApp as a listener
	midiIn.addListener(this);

	m_bDrawKeyboard = true;
	m_iKeyOffset = 12;
	m_iNumOctaves = 9;
    m_vecKeys.resize(12*m_iNumOctaves+1);
    ofSetFrameRate(60);
}

//--------------------------------------------------------------
void testApp::update() {

}

//--------------------------------------------------------------
void testApp::draw() {
    //ofBackground(ofColor(ofColor::white));
	ofSetColor(255);
	if(m_bDrawKeyboard){
        drawKeyboard();
	}

/*
	// draw the last recieved message contents to the screen
	text << "Received: " << ofxMidiMessage::getStatusString(midiMessage.status);
	ofDrawBitmapString(text.str(), 20, 20);
	text.str(""); // clear

	text << "channel: " << midiMessage.channel;
	ofDrawBitmapString(text.str(), 20, 34);
	text.str(""); // clear

	text << "pitch: " << midiMessage.pitch;
	ofDrawBitmapString(text.str(), 20, 48);
	text.str(""); // clear
	ofRect(20, 58, ofMap(midiMessage.pitch, 0, 127, 0, ofGetWidth()-40), 20);

	text << "velocity: " << midiMessage.velocity;
	ofDrawBitmapString(text.str(), 20, 96);
	text.str(""); // clear
	ofRect(20, 105, ofMap(midiMessage.velocity, 0, 127, 0, ofGetWidth()-40), 20);

	text << "control: " << midiMessage.control;
	ofDrawBitmapString(text.str(), 20, 144);
	text.str(""); // clear
	ofRect(20, 154, ofMap(midiMessage.control, 0, 127, 0, ofGetWidth()-40), 20);

	text << "value: " << midiMessage.value;
	ofDrawBitmapString(text.str(), 20, 192);
	text.str(""); // clear
	if(midiMessage.status == MIDI_PITCH_BEND) {
		ofRect(20, 202, ofMap(midiMessage.value, 0, MIDI_MAX_BEND, 0, ofGetWidth()-40), 20);
	}
	else {
		ofRect(20, 202, ofMap(midiMessage.value, 0, 127, 0, ofGetWidth()-40), 20);
	}

	text << "delta: " << midiMessage.deltatime;
	ofDrawBitmapString(text.str(), 20, 240);
	text.str(""); // clear

*/
}

//--------------------------------------------------------------
void testApp::drawKeyboard() {
    int idx = 0;

    ofPushMatrix();
    ofTranslate(10,60);
    ofPushStyle();
        // White Keys
        ofPushMatrix();
            for(int i=0;i<m_iNumOctaves;i++){
                for(int j=0; j<7;j++){
                    idx = 12*i+2*j;
                    if (j>2) --idx;
                    if (m_vecKeys[idx]) ofSetColor(ofColor::green);
                    else ofSetColor(ofColor::white);
                    ofRect(0,0,9,30);
                    ofTranslate(10,0);
                }
            }
        ofPopMatrix();

        // Black Keys
        ofPushMatrix();
            ofTranslate(5,0);
            for (int i=0;i<m_iNumOctaves;i++){
                for(int j=0; j<5;j++){
                    idx = 12*i + 2*j+1;
                    if (j>1) ++idx;
                    if(j==2) ofTranslate(10,0);
                    if (m_vecKeys[idx]) ofSetColor(ofColor::green);
                    else ofSetColor(ofColor::black);
                    ofRect(0,0,9,15);
                    ofTranslate(10,0);
                }
                ofTranslate(10,0);
            }
        ofPopMatrix();

        ofPushMatrix();
        ofTranslate(0,40);
            // Mod
            ofSetColor(ofColor::black);
            ofRect(0,0,m_iNumOctaves*7*10,10);
            ofSetColor(ofColor::red);
            ofRect(0,0,ofMap(m_iMod,0,127,0,m_iNumOctaves*7*10),10);
            // Pitch
            ofTranslate(0,15);
            ofSetColor(ofColor::black);
            ofRect(0,0,m_iNumOctaves*7*10,10);
            ofSetColor(ofColor::red);
            ofRect(0,0,ofMap(m_iPitch,0,16384,0,m_iNumOctaves*7*10),10);
            // Program
            ofTranslate(0,30);
            ofSetColor(ofColor::white);
            ofDrawBitmapString(m_sText.str(),0,0);
        ofPopMatrix();

        ofPushMatrix();
        ofTranslate(300,-20);
        ofPushStyle();
            ofFill();
            // CutOff
            drawPotentiometer(ofMap(m_iCutOff,0,127,0,TWO_PI));
            ofTranslate(40,0);
            // Resonance
            drawPotentiometer(ofMap(m_iResonance,0,127,0,TWO_PI));
            ofTranslate(40,0);
            // Attack
            drawPotentiometer(ofMap(m_iAttack,0,127,0,TWO_PI));
            ofTranslate(40,0);
            // Release
            drawPotentiometer(ofMap(m_iRelease,0,127,0,TWO_PI));
            ofTranslate(40,0);
            // Tempo
            drawPotentiometer(ofMap(m_fTempo,20,300,0,TWO_PI));
            ofTranslate(40,0);
        ofPopMatrix();
        ofPopStyle();

    ofPopStyle();
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::drawPotentiometer(float radAngle) {
    float i = HALF_PI;
    ofPolyline line;
    ofSetColor(ofColor::white);
    float r = 10.0;
    ofCircle(0,0,r);
    while (i < radAngle+HALF_PI) { // make a heart
        float x = cos(i) * r;
        float y = sin(i) * r;
        line.addVertex(ofVec2f(x,y));
        i+=0.005*radAngle*0.5;
    }
    line.addVertex(ofVec2f(0,0));
    line.close(); // close the shape
    ofSetLineWidth(3);
    ofSetColor(ofColor::black);
    line.draw();
}

//--------------------------------------------------------------
void testApp::exit() {
	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
}

//--------------------------------------------------------------
void testApp::newMidiMessage(ofxMidiMessage& msg) {
    // Ignore timing messages
    if (msg.status!=MIDI_TIME_CLOCK){
        // make a copy of the latest message
        midiMessage = msg;

        if(midiMessage.status==MIDI_NOTE_ON)
            m_vecKeys[midiMessage.pitch-m_iKeyOffset] = true;
        if(midiMessage.status==MIDI_NOTE_OFF)
            m_vecKeys[midiMessage.pitch-m_iKeyOffset] = false;
        if(midiMessage.status==MIDI_PITCH_BEND)
            m_iPitch = midiMessage.value;
        if(midiMessage.status==MIDI_CONTROL_CHANGE)
        {
            switch (midiMessage.control) {
            case 1:
                m_iMod = midiMessage.value;
                break;
            case 74:
                m_iCutOff = midiMessage.value;
                break;
            case 71:
                m_iResonance = midiMessage.value;
                break;
            case 73:
                m_iAttack = midiMessage.value;
                break;
            case 72:
                m_iRelease = midiMessage.value;
                break;
            }
        }

        if(midiMessage.status==MIDI_PROGRAM_CHANGE)
        {
            m_sText.str(""); // clear
            int value = midiMessage.value;
            if(value>=0 && value<8)
                m_sText << "TRANCE A." << value+1;
            if(value>=64 && value<72)
                m_sText << "TRANCE B." << value-64+1;
            if(value>=8 && value<16)
                m_sText << "TECHNO/HOUSE A." << value+1;
            if(value>=72 && value<80)
                m_sText << "TECHNO/HOUSE B." << value-72+1;
            if(value>=16 && value<24)
                m_sText << "ELECTRONICA A." << value+1;
            if(value>=80 && value<88)
                m_sText << "ELECTRONICA B." << value-80+1;
            if(value>=24 && value<32)
                m_sText << "DNB/BREAKS A." << value+1;
            if(value>=88 && value<96)
                m_sText << "DNB/BREAKS B." << value-88+1;
            if(value>=32 && value<40)
                m_sText << "HIPHOP/VINTAGE A." << value+1;
            if(value>=96 && value<104)
                m_sText << "HIPHOP/VINTAGE B." << value-96+1;
            if(value>=40 && value<48)
                m_sText << "RETRO A." << value+1;
            if(value>=104 && value<112)
                m_sText << "RETRO B." << value-104+1;
            if(value>=48 && value<56)
                m_sText << "S.E./HIT A." << value+1;
            if(value>=112 && value<120)
                m_sText << "S.E./HIT B." << value-112+1;
            if(value>=56 && value<64)
                m_sText << "VOCODER A." << value+1;
            if(value>=120 && value<128)
                m_sText << "VOCODER B." << value-120+1;
        }
    } else {
        float time = m_fTime;
        m_fTime = ofGetElapsedTimeMillis();
        m_fTempo = ofMap(1.0/(m_fTime-time),0.008,0.125,20,300);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	switch(key) {
		case 'l':
			midiIn.listPorts();
			break;
        case 'k' :
            m_bDrawKeyboard=!m_bDrawKeyboard;
            break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mouseReleased() {
}
