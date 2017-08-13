#include "stdafx.h"
#include "ImageMatch.h"


ImageMatch::ImageMatch()
{
}
void ImageMatch::match(Mat srcImage, Mat dstImage, int METHOD){
	switch (METHOD){
	case SURF:
		matchUseSURF(srcImage, dstImage);
		break;
	case SIFT:
		break;
	}
}
void ImageMatch::findRepetit(string filepath){
	
}
Mat ImageMatch::drawGoodMatches(
	const Mat& img1,
	const Mat& img2,
	const std::vector<KeyPoint>& keypoints1,
	const std::vector<KeyPoint>& keypoints2,
	std::vector<DMatch>& matches,
	std::vector<Point2f>& scene_corners_
	)
{
	//-- Sort matches and preserve top 10% matches
	std::sort(matches.begin(), matches.end());
	std::vector< DMatch > good_matches;
	double minDist = matches.front().distance;
	double maxDist = matches.back().distance;

	const int ptsPairs = std::min(GOOD_PTS_MAX, (int)(matches.size() * GOOD_PORTION));
	for (int i = 0; i < ptsPairs; i++)
	{
		good_matches.push_back(matches[i]);
	}
	//std::cout << "\nMax distance: " << maxDist << std::endl;
	//std::cout << "Min distance: " << minDist << std::endl;
		double sum = 0.0;
	for (int i = 0; i < good_matches.size(); i++){
		sum += good_matches.at(i).distance;
	}
	average_distance = sum / good_matches.size();
	std::cout << "平均距离:" << average_distance << endl;
	//std::cout << "Calculating homography using " << ptsPairs << " point pairs." << std::endl;

	// drawing the results
	Mat img_matches;

	drawMatches(img1, keypoints1, img2, keypoints2,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for (size_t i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints1[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints2[good_matches[i].trainIdx].pt);
	}
	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = Point(0, 0);
	obj_corners[1] = Point(img1.cols, 0);
	obj_corners[2] = Point(img1.cols, img1.rows);
	obj_corners[3] = Point(0, img1.rows);
	std::vector<Point2f> scene_corners(4);

	//Mat H = findHomography(obj, scene, RANSAC);
	//perspectiveTransform(obj_corners, scene_corners, H);

	//scene_corners_ = scene_corners;

	////-- Draw lines between the corners (the mapped object in the scene - image_2 )
	//line(img_matches,
	//	scene_corners[0] + Point2f((float)img1.cols, 0), scene_corners[1] + Point2f((float)img1.cols, 0),
	//	Scalar(0, 255, 0), 2, LINE_AA);
	//line(img_matches,
	//	scene_corners[1] + Point2f((float)img1.cols, 0), scene_corners[2] + Point2f((float)img1.cols, 0),
	//	Scalar(0, 255, 0), 2, LINE_AA);
	//line(img_matches,
	//	scene_corners[2] + Point2f((float)img1.cols, 0), scene_corners[3] + Point2f((float)img1.cols, 0),
	//	Scalar(0, 255, 0), 2, LINE_AA);
	//line(img_matches,
	//	scene_corners[3] + Point2f((float)img1.cols, 0), scene_corners[0] + Point2f((float)img1.cols, 0),
	//	Scalar(0, 255, 0), 2, LINE_AA);
	return img_matches;
}
double ImageMatch::getSimilarity(){
	return average_distance;
}
Mat ImageMatch::GetMatchedImage(){
	return matchedImage;
}
void ImageMatch::matchUseSURF(Mat srcImage, Mat dstImage){
	//namedWindow("surf matches", 0);
	//cout << "start" << endl;
	Mat image1, image2;
	int64 work_begin = 0;
	int64 work_end = 0;
	//cvtColor(srcImage,image1,CV_RGB2GRAY);
	//cvtColor(dstImage, image2, CV_RGB2GRAY);
	UMat img1,img2;
	srcImage.copyTo(img1);
	dstImage.copyTo(img2);
	double surf_time = 0.;
	//declare input/output
	std::vector<KeyPoint> keypoints1, keypoints2;
	std::vector<DMatch> matches;
	std::vector<DMatch> matches_flip;

	UMat _descriptors1, _descriptors2;
	Mat descriptors1 = _descriptors1.getMat(ACCESS_RW),
		descriptors2 = _descriptors2.getMat(ACCESS_RW);
	//instantiate detectors/matchers
	SURFDetector surf(100);

	SURFMatcher<BFMatcher> matcher;
	work_begin = getTickCount();
		surf(img1.getMat(ACCESS_READ), Mat(), keypoints1, descriptors1);
		surf(img2.getMat(ACCESS_READ), Mat(), keypoints2, descriptors2);
		matcher.match(descriptors1, descriptors2, matches);
		if (matches.size() > 0){
			work_end = getTickCount() - work_begin;
			//std::cout << "------------------------------------------------------------------------------" << endl;
			//std::cout << "FOUND " << keypoints1.size() << " keypoints on first image" << std::endl;
			//std::cout << "FOUND " << keypoints2.size() << " keypoints on second image" << std::endl;
			//std::cout << "FOUND " << matches.size() << " in matches" << endl;
			//surf_time = work_end / ((double)getTickFrequency())* 1000.;
			work_end = work_end / ((double)getTickFrequency())* 1000.;
			//std::cout << "SURF run time: " << surf_time / LOOP_NUM << " ms" << std::endl << "\n";
			//std::cout << "SURF run time: " << work_end << " ms" << std::endl << "\n";

			std::vector<Point2f> corner;
			Mat img_matches = drawGoodMatches(img1.getMat(ACCESS_READ), img2.getMat(ACCESS_READ), keypoints1, keypoints2, matches, corner);
			//drawMatches(img1.getMat(ACCESS_READ), keypoints1, img2.getMat(ACCESS_READ), keypoints2, match_result, img_matches);//进行绘制
			//-- Show detected matches
			
			matchedImage = img_matches;
			//imwrite("SURF.jpg", img_matches);
			//std::cout << "================================================================================" << endl;
		}
		//return;
}

vector< DMatch > ImageMatch::matchSimolify(vector< DMatch > input_dmatch1, vector< DMatch > input_dmatch2){
	vector<DMatch> matchs;
	for (int i = 0; i < input_dmatch1.size(); i++){
		if (input_dmatch2.at(input_dmatch1.at(i).trainIdx).trainIdx == input_dmatch1.at(i).queryIdx){
			matchs.push_back(input_dmatch1.at(i));
		}
	}
	return matchs;
}
ImageMatch::~ImageMatch()
{
}