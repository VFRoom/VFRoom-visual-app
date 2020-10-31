//################//
/*  Only Camera  */
//##############//

//#include <opencv2/opencv.hpp>
//#include <iostream>
//#include <string>
//
//using namespace cv;
//using namespace std;
//
//int main(int argc, char* argv[])
//{
//	// Open the Default Video Camera
//	VideoCapture cap(0);
//
//	// Not Success => exit Program
//	if (cap.isOpened() == false)
//	{
//		"Cannot Open the Video Camera" << endl;
//		cin.get(); //wait for any key press
//		return -1;
//	}
//
//	double dWidth = cap.get(CAP_PROP_FRAME_WIDTH); //get the width of video
//	double dHeight = cap.get(CAP_PROP_FRAME_HEIGHT); //get the height of video
//
//	"Resolution of the video : " << dWidth << " x " << dHeight << endl;
//
//	string windowName = "My Camera";
//	namedWindow(windowName); //create window called "My Camera"
//
//	while (true)
//	{
//		Mat frame;
//		bool bSuccess = cap.read(frame); //read a new frame from video
//
//		// Breaking While Loop if Frames cannot be Captured
//		if (bSuccess == false)
//		{
//			"Video Camera is Disconnected" << endl;
//			cin.get(); //wait for any key press
//			break;
//		}
//
//		// Show Frame in Created Window
//		imshow(windowName, frame);
//
//		// Wait for 10ms until any key is Pressed
//		// If 'Esc' is Pressed => break While Loop
//		// If any other key is Pressed => continue the Loop
//		// If any key is not Pressed within 10ms => continue the Loop
//		if (waitKey(10) == 27)
//		{
//			"Esc key is Pressed by User. Stoppig the Video" << endl;
//			break;
//		}
//	}
//
//	return 0;
//
//
//
//
//
//}



//###############//
/*  DETECT RED  */
//#############//

#include <iostream>
#include <opencv2/opencv.hpp>
// #include "opencv/build/include/opencv2/opencv.hpp"
// #include "./opencv/build/include/opencv2/opencv_modules.hpp"
// #include "opencv/build/include/opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	// Capture Video from Webcam
	//VideoCapture cap(0);

	// Read Image
	Mat personImage = imread("2.jpg");
	resize(personImage, personImage, Size(750, 800));

	// If no Image on Path
	if (!personImage.data)
	{
		cout << "No Image on this Path" << endl;
		return -1;
	}

	// If no Cam => exit Program
	//if (!cap.isOpened())
	//{
	//	cout << "Cannot Open Web Cam" << endl;
	//	return -1;
	//}

	// Create Window called "Control"
	namedWindow("Control"); //uncomment if need control panel

	// Start Settings for "Control"
	int iLowH = 0;
	int iHighH = 255;

	int iLowS = 0;
	int iHighS = 255;

	int iLowV = 0;
	int iHighV = 53;

	// Create Trackbars in "Control" Window
	// Hue (0 - 255)
	createTrackbar("LowH", "Control", &iLowH, 255);
	createTrackbar("HighH", "Control", &iHighH, 255);

	// Saturation (0 - 255)
	createTrackbar("LowS", "Control", &iLowS, 255);
	createTrackbar("HighS", "Control", &iHighS, 255);

	// Value (0 - 255)
	createTrackbar("LowV", "Control", &iLowV, 255);
	createTrackbar("HighV", "Control", &iHighV, 255);

	int iLastX = -1;
	int iLastY = -1;

	// Capture Temporary Image from Camera
	//Mat imgTmp;
	//personImage.read(imgTmp);

	// Create Black Image with Size as Сamera Output
	//Mat imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);

	while (true)
	{
		// Original of Camera
		//Mat imgOriginal;

		// Read new Frame from Video
		//bool bSuccess = personImage.read(imgOriginal);

		// If not Success => break Loop
		//if (!bSuccess)
		//{
		//	cout << "Cannot Read Frame from Video Stream" << endl;
		//	break;
		//}

		Mat imgHSV;

		// Convert Captured Frame from BGR to HSV
		cvtColor(personImage, imgHSV, COLOR_BGR2HSV);

		Mat imgThresholded;

		// Threshold Image
		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);

		// Morphological Opening (removes small objects from the foreground)
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		// Morphological Closing (removes small holes from the foreground)
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		// Calculate Moments of Thresholded Image
		Moments oMoments = moments(imgThresholded);

		double dM01 = oMoments.m01;
		double dM10 = oMoments.m10;
		double dArea = oMoments.m00;

		// if area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
		if (dArea > 10000)
		{
			// Calculate Position of Ball(or smth else)
			int posX = dM10 / dArea;
			int posY = dM01 / dArea;

			if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
			{
				// Draw Red Line from Previous Point to the Current Point
				//line(imageMaks, Point(posX, posY), Point(iLastX, iLastY), Scalar(0, 0, 255), 2); //uncomment to spawn red line
			}

			//iLastX = posX;
			//iLastY = posY;
		}

		// Show Thresholded Image
		imshow("Thresholded Image", imgThresholded); //uncomment if need black-white window

		// Show the Original Image
		//imgOriginal = imgOriginal + imgLines;
		//addWeighted(imgOriginal, 0.4, imageMaks, 0.6, 0);
		imshow("Original", personImage);

		// Image Window
		//imshow("IMG", imageMaks); //uncomment to see image

		// Wait for 'Esc' key press for 30ms
		// If 'Esc' key is pressed => break loop
		if (waitKey(30) == 27)
		{
			cout << "Esc key is Pressed by User" << endl;
			break;
		}
	}

	return 0;
}



//#####################//
//*  CUT BACKGROUND  *//
//###################//

//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc.hpp"
//
//#include <iostream>
//
//using namespace cv;
//using namespace std;
//
//static void help(char** argv)
//{
//	cout << "\nThis program demonstrates GrabCut segmentation -- select an object in a region\n"
//		"and then grabcut will attempt to segment it out.\n"
//		"Call:\n"
//		<< argv[0] << " <image_name>\n"
//		"\nSelect a rectangular area around the object you want to segment\n" <<
//		"\nHot keys: \n"
//		"\tESC - quit the program\n"
//		"\tr - restore the original image\n"
//		"\tn - next iteration\n"
//		"\n"
//		"\tleft mouse button - set rectangle\n"
//		"\n"
//		"\tCTRL+left mouse button - set GC_BGD pixels\n"
//		"\tSHIFT+left mouse button - set GC_FGD pixels\n"
//		"\n"
//		"\tCTRL+right mouse button - set GC_PR_BGD pixels\n"
//		"\tSHIFT+right mouse button - set GC_PR_FGD pixels\n" << endl;
//}
//
//const Scalar RED = Scalar(0, 0, 255);
//const Scalar PINK = Scalar(230, 130, 255);
//const Scalar BLUE = Scalar(255, 0, 0);
//const Scalar LIGHTBLUE = Scalar(255, 255, 160);
//const Scalar GREEN = Scalar(0, 255, 0);
//
//const int BGD_KEY = EVENT_FLAG_CTRLKEY;
//const int FGD_KEY = EVENT_FLAG_SHIFTKEY;
//
//static void getBinMask(const Mat& comMask, Mat& binMask)
//{
//	if (comMask.empty() || comMask.type() != CV_8UC1)
//		CV_Error(Error::StsBadArg, "comMask is empty or has incorrect type (not CV_8UC1)");
//	if (binMask.empty() || binMask.rows != comMask.rows || binMask.cols != comMask.cols)
//		binMask.create(comMask.size(), CV_8UC1);
//	binMask = comMask & 1;
//}
//
//class GCApplication
//{
//public:
//	enum { NOT_SET = 0, IN_PROCESS = 1, SET = 2 };
//	static const int radius = 2;
//	static const int thickness = -1;
//
//	void reset();
//	void setImageAndWinName(const Mat& _image, const string& _winName);
//	void showImage() const;
//	void mouseClick(int event, int x, int y, int flags, void* param);
//	int nextIter();
//	int getIterCount() const { return iterCount; }
//private:
//	void setRectInMask();
//	void setLblsInMask(int flags, Point p, bool isPr);
//
//	const string* winName;
//	const Mat* image;
//	Mat mask;
//	Mat bgdModel, fgdModel;
//
//	uchar rectState, lblsState, prLblsState;
//	bool isInitialized;
//
//	Rect rect;
//	vector<Point> fgdPxls, bgdPxls, prFgdPxls, prBgdPxls;
//	int iterCount;
//};
//
//void GCApplication::reset()
//{
//	if (!mask.empty())
//		mask.setTo(Scalar::all(GC_BGD));
//	bgdPxls.clear(); fgdPxls.clear();
//	prBgdPxls.clear();  prFgdPxls.clear();
//
//	isInitialized = false;
//	rectState = NOT_SET;
//	lblsState = NOT_SET;
//	prLblsState = NOT_SET;
//	iterCount = 0;
//}
//
//void GCApplication::setImageAndWinName(const Mat& _image, const string& _winName)
//{
//	if (_image.empty() || _winName.empty())
//		return;
//	image = &_image;
//	winName = &_winName;
//	mask.create(image->size(), CV_8UC1);
//	reset();
//}
//
//void GCApplication::showImage() const
//{
//	if (image->empty() || winName->empty())
//		return;
//
//	Mat res;
//	Mat binMask;
//	if (!isInitialized)
//		image->copyTo(res);
//	else
//	{
//		getBinMask(mask, binMask);
//		image->copyTo(res, binMask);
//	}
//
//	vector<Point>::const_iterator it;
//	for (it = bgdPxls.begin(); it != bgdPxls.end(); ++it)
//		circle(res, *it, radius, BLUE, thickness);
//	for (it = fgdPxls.begin(); it != fgdPxls.end(); ++it)
//		circle(res, *it, radius, RED, thickness);
//	for (it = prBgdPxls.begin(); it != prBgdPxls.end(); ++it)
//		circle(res, *it, radius, LIGHTBLUE, thickness);
//	for (it = prFgdPxls.begin(); it != prFgdPxls.end(); ++it)
//		circle(res, *it, radius, PINK, thickness);
//
//	if (rectState == IN_PROCESS || rectState == SET)
//		rectangle(res, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), GREEN, 2);
//
//	imshow(*winName, res);
//}
//
//void GCApplication::setRectInMask()
//{
//	CV_Assert(!mask.empty());
//	mask.setTo(GC_BGD);
//	rect.x = max(0, rect.x);
//	rect.y = max(0, rect.y);
//	rect.width = min(rect.width, image->cols - rect.x);
//	rect.height = min(rect.height, image->rows - rect.y);
//	(mask(rect)).setTo(Scalar(GC_PR_FGD));
//}
//
//void GCApplication::setLblsInMask(int flags, Point p, bool isPr)
//{
//	vector<Point>* bpxls, * fpxls;
//	uchar bvalue, fvalue;
//	if (!isPr)
//	{
//		bpxls = &bgdPxls;
//		fpxls = &fgdPxls;
//		bvalue = GC_BGD;
//		fvalue = GC_FGD;
//	}
//	else
//	{
//		bpxls = &prBgdPxls;
//		fpxls = &prFgdPxls;
//		bvalue = GC_PR_BGD;
//		fvalue = GC_PR_FGD;
//	}
//	if (flags & BGD_KEY)
//	{
//		bpxls->push_back(p);
//		circle(mask, p, radius, bvalue, thickness);
//	}
//	if (flags & FGD_KEY)
//	{
//		fpxls->push_back(p);
//		circle(mask, p, radius, fvalue, thickness);
//	}
//}
//
//void GCApplication::mouseClick(int event, int x, int y, int flags, void*)
//{
//	// TODO add bad args check
//	switch (event)
//	{
//	case EVENT_LBUTTONDOWN: // set rect or GC_BGD(GC_FGD) labels
//	{
//		bool isb = (flags & BGD_KEY) != 0,
//			isf = (flags & FGD_KEY) != 0;
//		if (rectState == NOT_SET && !isb && !isf)
//		{
//			rectState = IN_PROCESS;
//			rect = Rect(x, y, 1, 1);
//		}
//		if ((isb || isf) && rectState == SET)
//			lblsState = IN_PROCESS;
//	}
//	break;
//	case EVENT_RBUTTONDOWN: // set GC_PR_BGD(GC_PR_FGD) labels
//	{
//		bool isb = (flags & BGD_KEY) != 0,
//			isf = (flags & FGD_KEY) != 0;
//		if ((isb || isf) && rectState == SET)
//			prLblsState = IN_PROCESS;
//	}
//	break;
//	case EVENT_LBUTTONUP:
//		if (rectState == IN_PROCESS)
//		{
//			rect = Rect(Point(rect.x, rect.y), Point(x, y));
//			rectState = SET;
//			setRectInMask();
//			CV_Assert(bgdPxls.empty() && fgdPxls.empty() && prBgdPxls.empty() && prFgdPxls.empty());
//			showImage();
//		}
//		if (lblsState == IN_PROCESS)
//		{
//			setLblsInMask(flags, Point(x, y), false);
//			lblsState = SET;
//			showImage();
//		}
//		break;
//	case EVENT_RBUTTONUP:
//		if (prLblsState == IN_PROCESS)
//		{
//			setLblsInMask(flags, Point(x, y), true);
//			prLblsState = SET;
//			showImage();
//		}
//		break;
//	case EVENT_MOUSEMOVE:
//		if (rectState == IN_PROCESS)
//		{
//			rect = Rect(Point(rect.x, rect.y), Point(x, y));
//			CV_Assert(bgdPxls.empty() && fgdPxls.empty() && prBgdPxls.empty() && prFgdPxls.empty());
//			showImage();
//		}
//		else if (lblsState == IN_PROCESS)
//		{
//			setLblsInMask(flags, Point(x, y), false);
//			showImage();
//		}
//		else if (prLblsState == IN_PROCESS)
//		{
//			setLblsInMask(flags, Point(x, y), true);
//			showImage();
//		}
//		break;
//	}
//}
//
//int GCApplication::nextIter()
//{
//	if (isInitialized)
//		grabCut(*image, mask, rect, bgdModel, fgdModel, 1);
//	else
//	{
//		if (rectState != SET)
//			return iterCount;
//
//		if (lblsState == SET || prLblsState == SET)
//			grabCut(*image, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_MASK);
//		else
//			grabCut(*image, mask, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_RECT);
//
//		isInitialized = true;
//	}
//	iterCount++;
//
//	bgdPxls.clear(); fgdPxls.clear();
//	prBgdPxls.clear(); prFgdPxls.clear();
//
//	return iterCount;
//}
//
//GCApplication gcapp;
//
//static void on_mouse(int event, int x, int y, int flags, void* param)
//{
//	gcapp.mouseClick(event, x, y, flags, param);
//}
//
//int main(int argc, char** argv)
//{
//	cv::CommandLineParser parser(argc, argv, "{@input| messi5.jpg |}");
//	help(argv);
//
//	string filename = parser.get<string>("@input");
//	if (filename.empty())
//	{
//		cout << "\nDurn, empty filename" << endl;
//		return 1;
//	}
//	Mat image = imread(samples::findFile(filename), IMREAD_COLOR);
//	if (image.empty())
//	{
//		cout << "\n Durn, couldn't read image filename " << filename << endl;
//		return 1;
//	}
//
//
//	// Open the Default Video Camera
//	VideoCapture cap(0);
//	if (cap.isOpened() == false)
//	{
//		"Cannot Open the Video Camera" << endl;
//		cin.get(); //wait for any key press
//		return -1;
//	}
//	Mat frame;
//	bool bSuccess = cap.read(frame); //read a new frame from video
//	// Breaking While Loop if Frames cannot be Captured
//	if (bSuccess == false)
//	{
//		"Video Camera is Disconnected" << endl;
//		cin.get(); //wait for any key press
//		return -1;
//	}
//
//
//	const string winName = "image";
//	namedWindow(winName, WINDOW_AUTOSIZE);
//	setMouseCallback(winName, on_mouse, 0);
//
//	gcapp.setImageAndWinName(frame, winName);
//	gcapp.showImage();
//
//	for (;;)
//	{
//		char c = (char)waitKey(0);
//		switch (c)
//		{
//		case '\x1b':
//			cout << "Exiting ..." << endl;
//			goto exit_main;
//		case 'r':
//			cout << endl;
//			gcapp.reset();
//			gcapp.showImage();
//			break;
//		case 'n':
//			int iterCount = gcapp.getIterCount();
//			cout << "<" << iterCount << "... ";
//			int newIterCount = gcapp.nextIter();
//			if (newIterCount > iterCount)
//			{
//				gcapp.showImage();
//				cout << iterCount << ">" << endl;
//			}
//			else
//				cout << "rect must be determined>" << endl;
//			break;
//		}
//	}
//
//exit_main:
//	destroyWindow(winName);
//	return 0;
//}
