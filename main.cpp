//
//  main.cpp
//
//  Created by Jean-Marie Normand on 12/02/13.
//  Copyright (c) 2013 Centrale Nantes. All rights reserved.
//


// OpenGL/GLUT
#include <Windows.h>
#include <GL/gl.h>
#include <GL/GLUT.h>

// Main include
#include "main.h"


// Initializing OpenGL/GLUt states
void initGL(int argc, char * argv[]) {
   
   // Setting window's initial position
   glutInitWindowPosition(0, 0);
   // Setting initial window's size: initialized with the size of the video
   glutInitWindowSize(widthFrame,heightFrame);
   
   // Setting up display mode
   glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );
   // Creating window and setting its name
   g_hWindow = glutCreateWindow( "AruCo" );
   // setting up draw callback
   glutDisplayFunc( doWork );
   // Setting up idle callback
   glutIdleFunc( idle );
   // Setting up resize callback
   glutReshapeFunc( resize );
   // Setting up the mouse callback
   glutMouseFunc( mouse );
   // Setting up keyboard callback
   glutKeyboardFunc( keyboard );
   
   // Setting up clear color
   glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
   // and depth clear value
   glClearDepth( 1.0 );
   
   glShadeModel (GL_SMOOTH);
   glEnable(GL_NORMALIZE);
   
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
}

// Resize function
void resize(GLsizei iWidth, GLsizei iHeight) {
   // Calling ArUco resize
   arucoManager->resize(iWidth, iHeight);
   
   // not all sizes are allowed. OpenCV images have padding
   // at the end of each line in these that are not aligned to 4 bytes
   if (iWidth*3%4!=0) {
      iWidth+=iWidth*3%4;//resize to avoid padding
   }
   glutReshapeWindow(iWidth, iHeight);
   glutPostRedisplay();
}

// Mouse function
void mouse(int b,int s,int x,int y) {
   // nothing for now
}

// Keyboard function
void keyboard(unsigned char key, int x, int y) {
   switch (key) {
      case KEY_ESCAPE:
         exitFunction();
         exit(0);
         break;

      default:
         break;
	}
	
}

// Loop function
void doWork() {

   // Showing images
   //imshow(windowNameCapture, curImg);
   
   // Calling ArUco draw function
   arucoManager->draw3DAxisResizedImg(0);
   arucoManager->drawScene();
   
   // Swapping GLUT buffers
   glutSwapBuffers();
}

// Idle function
void idle() {
   
   // Clearing color and depth buffers
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   
   // Getting current frames
   cap >> curImg;
   
   // Calling ArUco idle
   arucoManager->idle(curImg);
    
   // Keyboard manager + waiting for key
   char retKey = cv::waitKey(1);
   if(retKey == KEY_ESCAPE){
      exitFunction();
      exit(EXIT_SUCCESS);
   }
  
   // Redisplay GLUT window
   glutPostRedisplay();
}


// Exit function
void exitFunction() {  
   
   // Destroy OpenCV window
   destroyWindow(windowNameCapture);
   
   // Release capture
   cap.release();
   
   // Deleting ArUco manager
   if(arucoManager) {
      delete(arucoManager);
      arucoManager = NULL;
   }
   
   // Deleting GLUT window
   if(g_hWindow) {
      glutDestroyWindow(g_hWindow);
   }
   
}

// Main 
int main(int argc, char * argv[])
{
   // OpenGL/GLUT Initialization
   glutInit(&argc, argv);
   
   // print a welcome message, and the OpenCV version
   printf ("Welcome to arucoMinimal, using OpenCV version %s (%d.%d.%d)\n",
           CV_VERSION,
           CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION);
   
   printf("Hot keys: \n"
          "\tESC - quit the program\n");
   
   // Creating the ArUco object
   arucoManager = new ArUco("camera.yml", 0.105f);
   std::cout<<"ArUco OK"<<std::endl;
   
   // Creating the OpenCV capture
   cout << "Entrez l'identifiant de la camera" << endl;
   cin >> cameraID;
   cap.open(cameraID);
   if(!cap.isOpened()) {
      cerr << "Erreur lors de l'initialisation de la capture de la camera !"<< endl;
      cerr << "Fermeture..." << endl;
      exit(EXIT_FAILURE);
   }
   else{
      // retrieving a first frame so that the display does not crash
      cap >> curImg;
   }
   
   // Getting width/height of the image
   widthFrame  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
   heightFrame = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
   std::cout<<"Frame width = "<<widthFrame<<std::endl;
   std::cout<<"Frame height = "<<heightFrame<<std::endl;
   
   // OpenCV window
   //windowNameCapture = "Scene";
   //cv::namedWindow(windowNameCapture, CV_WINDOW_AUTOSIZE);  
   
   // Exit function
   atexit(exitFunction);
   
   // OpenGL/GLUT Initialization
   initGL(argc, argv);
   
   // Starting GLUT main loop
   glutMainLoop();
   
   return 0;
}


	/*
//Question 1
#define Q_KEY        113
#define ESC_KEY      27

int main(int argc, char * argv[])
{
	Mat im;
	int key=0;
	string imName;
	imName = "Z:\Cours\VSION\ARCUO\marqueurs\lasthope.png" ;
	// If we give an argument then open it instead of Lena
	if(argc == 2) {
		 imName = argv[1];
	}
	
	 im = imread(imName, CV_LOAD_IMAGE_COLOR);

	 MarkerDetector	myDetector;
	 vector <Marker> markers;
	 myDetector.detect(im,markers);
	 cout<<markers.size()<<endl;
	 for (unsigned int i=0;i<markers.size();i++) {
		cout << markers[i];
		markers[i].draw(im, Scalar(0,0,255),2);
	}
	 string WindowsName = "Test0";
 	 imshow(WindowsName, im);

	 while ( (key != ESC_KEY) && (key!= Q_KEY) ) {
		key = waitKey(1);
	}

 	destroyAllWindows();


}
*/

//
//  main.h
//
//  Created by Jean-Marie Normand on 01/03/13.
//  Copyright (c) 2013 Centrale Nantes. All rights reserved.
//

#ifndef UserPerspectiveAR_main_h
#define UserPerspectiveAR_main_h
#include <Windows.h>
#include <iostream>

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>

// OpenGL/GLUT
#include <GL/gl.h>
#include <GL/GLUT.h>

// OpenCV
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// ArUco
#include "ArUco-OpenGL.h"

// Default wdth and height of the video
#define DEFAULT_VIDEO_WIDTH   800
#define DEFAULT_VIDEO_HEIGHT  600

// Defining escape key
#define KEY_ESCAPE 27
#define DEFAULT_RESIZE_RATIO 0.7

// Namespaces
using namespace std;
using namespace cv;

// OpenCV stuff: cameras, etc.
int            cameraID;
VideoCapture   cap;

// Names of the OpenCV windows
string         windowNameCapture;


// Width/Height of the image
int            widthFrame;
int            heightFrame;

// GLUT attributes
GLint          g_hWindow;

// ArUco object
ArUco          *arucoManager;

// Ratio of view resizing
double         resizeRatio;

// Keeping current capture image
cv::Mat        curImg;

// Test again
cv::Mat        debugImg;

// Forward declarations

// GLUT functions

// Initializing OpenGL
void initGL(int argc, char * argv[]);

// Loop function
void doWork();

// Idle function
void idle();

// Resize function
void resize(GLsizei iWidth, GLsizei iHeight);

// Mouse function
void mouse(int b,int s,int x,int y);

// Keyboard function
void keyboard(unsigned char key, int x, int y);

// Exit function
void exitFunction();

#endif

//
//  ArUco-OpenGL.cpp
//
//  Created by Jean-Marie Normand on 28/02/13.
//  Copyright (c) 2013 Centrale Nantes. All rights reserved.
//
#include <Windows.h>
#include <iostream>
#include "ArUco-OpenGL.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <Gl/GL.h>
#include <GL/GLUT.h>


using namespace std;
using namespace cv;
// Constructor
ArUco::ArUco(string intrinFileName, float markerSize) {
   // Initializing attributes
   m_IntrinsicFile= intrinFileName;
   m_MarkerSize   = markerSize;
   // read camera parameters if passed
   m_CameraParams.readFromXMLFile(intrinFileName);
}

// Destructor
ArUco::~ArUco() {}

// Detect marker and draw things
void ArUco::doWork(Mat inputImg) {
   m_InputImage   = inputImg;
   m_GlWindowSize = m_InputImage.size();
   resize(m_GlWindowSize.width, m_GlWindowSize.height);
}

// Draw axis function
void ArUco::drawAxis(float axisSize) {
   // X
   glColor3f (1,0,0);
   glBegin(GL_LINES);
   glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
   glVertex3f(axisSize,0.0f, 0.0f); // ending point of the line
   glEnd( );
   
   // Y
   glColor3f (0,1,0);
   glBegin(GL_LINES);
   glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
   glVertex3f(0.0f, axisSize, 0.0f); // ending point of the line
   glEnd( );
   
   // Z
   glColor3f (0,0,1);
   glBegin(GL_LINES);
   glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
   glVertex3f(0.0f, 0.0f, axisSize); // ending point of the line
   glEnd( );
}

// GLUT functionnalities

// Drawing function
void ArUco::drawScene() {
   // If we do not have an image we don't do anyhting
   if (m_ResizedImage.rows==0)
      return;
   
   // Setting up  OpenGL matrices
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, m_GlWindowSize.width, 0, m_GlWindowSize.height, -1.0, 1.0);
   glViewport(0, 0, m_GlWindowSize.width , m_GlWindowSize.height);
   glDisable(GL_TEXTURE_2D);
   glPixelZoom( 1, -1);
   
   //////glRasterPos3f( 0, m_GlWindowSize.height  - 0.5f, -1.0f );
   glRasterPos3f(0, m_GlWindowSize.height, -1.0f);
   
   glDrawPixels (m_GlWindowSize.width, m_GlWindowSize.height, GL_RGB, GL_UNSIGNED_BYTE, m_ResizedImage.ptr(0));
   
   // Enabling depth test
   glEnable(GL_DEPTH_TEST);
   
   // Set the appropriate projection matrix so that rendering is done 
   // in an environment like the real camera (without distorsion)
   glMatrixMode(GL_PROJECTION);
   double proj_matrix[16];
   //m_CameraParams.glGetProjectionMatrix(m_InputImage.size(),m_GlWindowSize,proj_matrix,0.05,10);
   m_CameraParams.glGetProjectionMatrix(m_ResizedImage.size(),m_GlWindowSize,proj_matrix,0.05,10);
   glLoadIdentity();
   glLoadMatrixd(proj_matrix);
   
   // Debug : outputting projection matrix
   /*
    std::cout<<"Projection Matrix"<<std::endl;
   for(int i=0;i<4;i++) {
      std::cout<<"| ";
      for(int j=0;j<4;j++) {
         std::cout<<proj_matrix[j*4+i]<<"\t\t\t";
      }
      std::cout<<" |"<<std::endl;
   }
    */
   
   //now, for each marker,
   double modelview_matrix[16];
   std::cout << "Number of markers: " << m_Markers.size() << std::endl;
   
   // For each detected marker
   for (unsigned int m=0;m<m_Markers.size();m++)
   {
      m_Markers[m].glGetModelViewMatrix(modelview_matrix);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glLoadMatrixd(modelview_matrix);
      // Disabling light if it is on
      GLboolean lightOn = false;
      glGetBooleanv(GL_LIGHTING, &lightOn);
      if(lightOn) {
         glDisable(GL_LIGHTING);
      }
    
	// Drawing axis
	//drawAxis(m_MarkerSize);
	        
    //D�finition de la couleur en fonction du nombres de marqueurs
    glColor3f(pow(0.8,m_Markers.size()),pow(0.9,m_Markers.size()),pow(0.99,m_Markers.size()));
	
	
	//glTranslatef(0, m_MarkerSize/2,0);
    //Drawing a sphere
	//glutWireSphere ( m_MarkerSize/2,10,10);
	///Drawing a Teapot
    //glutWireTeapot( m_MarkerSize/2);
	
	//glPushMatrix();


      // Re-enabling light if it is on
      if(lightOn) {
         glEnable(GL_LIGHTING);
      }
   //glPopMatrix();
      
   }
 

	 
   
   // Disabling depth test
   glDisable(GL_DEPTH_TEST);
   
   m_pixels.create(m_GlWindowSize.height , m_GlWindowSize.width, CV_8UC3);
   //use fast 4-byte alignment (default anyway) if possible
   glPixelStorei(GL_PACK_ALIGNMENT, (m_pixels.step & 3) ? 1 : 4);
   //set length of one complete row in destination data (doesn't need to equal img.cols)
   glPixelStorei(GL_PACK_ROW_LENGTH, m_pixels.step/m_pixels.elemSize());
   // Reading back the pixels
   glReadPixels(0, 0, m_GlWindowSize.width , m_GlWindowSize.height, GL_RGB, GL_UNSIGNED_BYTE, m_pixels.data);
   // Flip the pixels since OpenCV stores top to bottom and OpenGL from bottom to top
   cv::flip(m_pixels, m_pixels, 0);
}

// Idle function
void ArUco::idle(Mat newImage) {
   // Getting new image
   m_InputImage = newImage.clone();
   
   // Do that here ?
   resize(m_InputImage.size().width, m_InputImage.size().height);
   
   // Undistort image based on distorsion parameters
   m_UndInputImage.create(m_InputImage.size(),CV_8UC3);
   
   //transform color that by default is BGR to RGB because windows systems do not allow reading BGR images with opengl properly
   cv::cvtColor(m_InputImage,m_InputImage,CV_BGR2RGB);
   
   //remove distorion in image
   // Jim commented next line and added the clone line
   //cv::undistort(m_InputImage,m_UndInputImage, m_CameraParams.CameraMatrix, m_CameraParams.Distorsion);
   m_UndInputImage = m_InputImage.clone();
   
   //detect markers
   m_PPDetector.detect(m_UndInputImage, m_Markers, m_CameraParams.CameraMatrix, Mat(), m_MarkerSize);
   //m_PPDetector.detect(m_UndInputImage, m_Markers, m_CameraParams, m_MarkerSize);
   
   //resize the image to the size of the GL window
   cv::resize(m_UndInputImage,m_ResizedImage,m_GlWindowSize);
}

// Resize function
void ArUco::resize(GLsizei iWidth, GLsizei iHeight) {
   m_GlWindowSize=Size(iWidth,iHeight);
   
   //not all sizes are allowed. OpenCv images have padding at the end of each line in these that are not aligned to 4 bytes
   if (iWidth*3%4!=0) {
      iWidth+=iWidth*3%4;//resize to avoid padding
      resize(iWidth, m_GlWindowSize.height);
   }
   else {
      //resize the image to the size of the GL window
      //if (m_UndInputImage.rows!=0)
         //cv::resize(m_UndInputImage, m_ResizedImage, m_GlWindowSize);
   }
}


// Jim
cv::Mat ArUco::getPixels() {
   return m_pixels.clone();
}

// Test using ArUco to display a 3D cube in OpenCV
void ArUco::draw3DCube(cv::Mat img, int markerInd) {
   if(m_Markers.size() > markerInd) {
      aruco::CvDrawingUtils::draw3dCube(img, m_Markers[markerInd], m_CameraParams); 
   }
}

void ArUco::draw3DAxis(cv::Mat img, int markerInd) {
   if(m_Markers.size() > markerInd) {
      aruco::CvDrawingUtils::draw3dAxis(img, m_Markers[markerInd], m_CameraParams); 
   }
   
}

void ArUco::draw3DAxisResizedImg(int markerInd) {
 if(m_Markers.size() > markerInd) {

	Marker m=m_Markers[markerInd];
	CameraParameters CP=m_CameraParams;
	Mat objectPoints (4,3,CV_32FC1);
    objectPoints.at<float>(0,0)=0;
    objectPoints.at<float>(0,1)=0;
    objectPoints.at<float>(0,2)=0;
    objectPoints.at<float>(1,0)=m.ssize;
    objectPoints.at<float>(1,1)=0;
    objectPoints.at<float>(1,2)=0;
    objectPoints.at<float>(2,0)=0;
    objectPoints.at<float>(2,1)=m.ssize;
    objectPoints.at<float>(2,2)=0;
    objectPoints.at<float>(3,0)=0;
    objectPoints.at<float>(3,1)=0;
    objectPoints.at<float>(3,2)=m.ssize;
    vector<Point2f> imagePoints(3,Point2f(0,0));
    cv::projectPoints( objectPoints, m.Rvec,m.Tvec, CP.CameraMatrix,CP.Distorsion,   imagePoints);

	int s = m_Markers.size();
	stringstream text;
	text << s;
	putText(m_ResizedImage,  text.str() , imagePoints[3],FONT_HERSHEY_SIMPLEX, 2, Scalar(255,0,0),2);

 }
}

//
//  ArUco-OpenGL.h
//
//  Created by Jean-Marie Normand on 28/02/13.
//  Copyright (c) 2013 Centrale Nantes. All rights reserved.
//

#ifndef UserPerspectiveAR_ArUco_OpenGL_h
#define UserPerspectiveAR_ArUco_OpenGL_h
#include <Windows.h>
#include <iostream>

#include <fstream>
#include <sstream>
#ifdef __APPLE__
   #include <OPENGL/gl.h>
   #include <GLUT/glut.h>
#else
   #include <GL/gl.h>
   #include <GL/glut.h>
#endif
#include <aruco.h>


using namespace cv;
using namespace aruco;

class ArUco {
// Attributes
protected:
   // Intrinsics file for the camera
   string            m_IntrinsicFile;
   // Size of the marker (in meters)
   float             m_MarkerSize;
   // The Marker Detector
   MarkerDetector    m_PPDetector;
   
   // Vector of detected markers in the image
   vector<Marker>    m_Markers;
   
   // OpenCV matrices storing the images
   // Input Image
   Mat               m_InputImage;
   // Undistorted image
   Mat               m_UndInputImage;
   // Resized image
   Mat               m_ResizedImage;
   // Camera parameters
   CameraParameters  m_CameraParams;
   // Size of the OpenGL window size
   Size              m_GlWindowSize;
   
   // Test to read OpenGL framebuffer
   Mat               m_pixels;
   
// Methods
public:
   // Constructor
   ArUco(string intrinFileName, float markerSize);
   // Destructor
   ~ArUco();
   
   // Detect marker and draw things
   void  doWork(Mat inputImg);
   
   // Draw axis function
   void  drawAxis(float axisSize);
   
   // GLUT functionnalities
   
   // Drawing function
   void  drawScene();
   
   // Idle function
   void  idle(Mat newImage);
   
   // Resize function
   void  resize(GLsizei iWidth, GLsizei iHeight);
   
   // Jim
   Mat   getPixels();
   
   void  setDesiredImageSize(Size desiredSize);
   void  setDesiredImageSize(int desiredWidth, int desiredHeight);
   
   // Test using ArUco to display a 3D cube in OpenCV
   void  draw3DCube(cv::Mat img, int markerInd=0);
   void  draw3DAxis(cv::Mat img, int markerInd=0);
   void  draw3DAxisResizedImg(int markerInd=0);
};


#endif

%YAML:1.0
calibration_time: "Thu Feb 21 15:54:16 2013"
image_width: 1280
image_height: 720
board_width: 11
board_height: 7
square_size: 2.
flags: 0
camera_matrix: !!opencv-matrix
   rows: 3
   cols: 3
   dt: d
   data: [ 1.0211322713071506e+03, 0., 6.2125214109119736e+02, 0.,
       1.0169485457435253e+03, 3.8332440294619215e+02, 0., 0., 1. ]
distortion_coefficients: !!opencv-matrix
   rows: 5
   cols: 1
   dt: d
   data: [ -1.7033512511308585e-01, 1.3685930369707442e+00,
       1.0159415571776863e-02, -1.7548664843997919e-02,
       -3.2133861342066212e+00 ]
avg_reprojection_error: 6.0018764652931189e-01
