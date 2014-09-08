#include "ofMain.h"
#include "testApp.h"

int main(){
	ofSetupOpenGL(800, 600, OF_WINDOW);
	ofRunApp(new testApp());
}
