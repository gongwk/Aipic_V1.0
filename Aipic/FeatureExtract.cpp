#include "stdafx.h"
#include "FeatureExtract.h"


FeatureExtract::FeatureExtract()
{
}
FeatureExtract::~FeatureExtract()
{
}
Mat FeatureExtract::Extract(Mat input_image, int method) {
	switch (method) {
	case _SURF:
		return SURF(input_image);
		break;
	case _SIFT:
		return SIFT(input_image);
		break;
	case _ORB:
		return ORB(input_image);
		break;
	case _HIST:
		return Hist(input_image);
	default:
		break;
	}
}
Mat FeatureExtract::SURF(
	Mat input_image,
	double 	hessianThreshold,
	int 	nOctaves,
	int 	nOctaveLayers,
	bool 	extended,
	bool 	upright,
	Scalar  scalar,
	int drawFlags) 
{
	Ptr<cv::xfeatures2d::SURF> detector = cv::xfeatures2d::SURF::create(hessianThreshold, nOctaves, nOctaveLayers, extended, upright);
	std::vector<KeyPoint> key_points;
	detector->detect(input_image, key_points);
	feature_keypoints.push_back(key_points);
	detector->compute(input_image, key_points, descriptor);
	Mat feature_image;
	drawKeypoints(input_image, key_points, feature_image, scalar, drawFlags);
	return feature_image;
}
Mat FeatureExtract::SIFT(Mat input_image,int nfeatures, int nOctaveLayers, double contrastThreshold, double edgeThreshold, double sigma) {
	int minHessian = 400;
	Ptr<cv::xfeatures2d::SIFT> detector = SIFT::create(nfeatures, nOctaveLayers,contrastThreshold,edgeThreshold,sigma);
	std::vector<KeyPoint> key_points;
	detector->detect(input_image, key_points);
	feature_keypoints.push_back(key_points);
	Mat feature_image;
	drawKeypoints(input_image, key_points, feature_image, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	return feature_image;
}
Mat FeatureExtract::ORB(
	Mat input_image,
	int 	nfeatures,
	float 	scaleFactor,
	int 	nlevels,
	int 	edgeThreshold,
	int 	firstLevel,
	int 	WTA_K,
	int 	scoreType,
	int 	patchSize,
	int 	fastThreshold,
	Scalar  scalar,
	int		drawFlags)
{
	Ptr<cv::ORB> detector = cv::ORB::create(nfeatures, scaleFactor, nlevels, edgeThreshold, firstLevel, WTA_K, scoreType, patchSize, fastThreshold);
	std::vector<KeyPoint> key_points;
	detector->detect(input_image, key_points);
	//if (key_points.size() < nfeatures / 2) return " ";
	//if (key_points.size() == 0) {
	//	return input_image;
	//}
	//if (key_points.size() != nfeatures) {
	//	for (int i = 0; i < (nfeatures - num); i++) {
	//		key_points.push_back(key_points.at(0));
	//	}
	//}
	feature_keypoints.push_back(key_points);
	detector->compute(input_image, key_points, descriptor);
	Mat feature_image;
	drawKeypoints(input_image, key_points, feature_image, scalar, drawFlags);
	return feature_image;
}

vector<Mat> FeatureExtract::ComputeDescriptor(vector<Mat> images, vector<vector<KeyPoint>> keypoints) {
	//只是用以计算特征描述子
	Ptr<SurfDescriptorExtractor> extractor = SURF::create();
	vector<Mat> descriptors;
	//extractor->compute(images, keypoints, descriptors);
	for (int i = 0; i < images.size(); i++) {
		//TODO:为什么必须放在这里，放在外面就有问题
		Mat des;
		extractor->compute(images.at(i), keypoints.at(i), des);
		
		//cout << "SurfDescriptorExtractor Size:" << des.size() << endl;
		descriptors.push_back(des);
	}
	return descriptors;
}
vector<Mat> FeatureExtract::ComputeSIFTDescriptor(vector<Mat> images, vector<vector<KeyPoint>> keypoints) {
	Ptr<SiftDescriptorExtractor> extractor = SIFT::create();
	vector<Mat> descriptors;
	for (int i = 0; i < images.size(); i++) {
		Mat des;
		extractor->compute(images.at(i), keypoints.at(i), des);
		descriptors.push_back(des);
	}
	return descriptors;
}
vector<Mat> FeatureExtract::ComputeSURFDescriptor(vector<Mat> images, vector<vector<KeyPoint>> keypoints) {
	Ptr<SurfDescriptorExtractor> extractor = SURF::create();
	vector<Mat> descriptors;
	for (int i = 0; i < images.size(); i++) {
		Mat des;
		extractor->compute(images.at(i), keypoints.at(i), des);
		descriptors.push_back(des);
	}
	return descriptors;
}
vector<Mat> FeatureExtract::ComputeORBDescriptor(vector<Mat> images, vector<vector<KeyPoint>> keypoints) {
	Ptr<cv::ORB> extractor = cv::ORB::create();
	vector<Mat> descriptors;
	for (int i = 0; i < images.size(); i++) {
		Mat des;
		extractor->compute(images.at(i), keypoints.at(i), des);
		descriptors.push_back(des);
	}
	return descriptors;
}
vector<KeyPoint> FeatureExtract::OptimizeFeature(vector<KeyPoint> keypoint, int cut_number) {
	if (keypoint.size()<3*cut_number)
	{
		return keypoint;
	}
	//计算图片的特征点重心，依据每个特征点到重心的距离对特征点排序,由近到远
	vector<double> L2;//距离
	double mid_x = 0.0;
	double mid_y = 0.0;
	for (int j = 0; j < keypoint.size(); j++) {
		mid_x += keypoint.at(j).pt.x;
		mid_y += keypoint.at(j).pt.y;
	}
	mid_x = mid_x / keypoint.size();
	mid_y = mid_y / keypoint.size();
	//距离计算
	for (int j = 0; j < keypoint.size(); j++) {
		L2.push_back(sqrt(pow((mid_x - keypoint.at(j).pt.x), 2) + pow((mid_y - keypoint.at(j).pt.y), 2)));
	}
	//排序，按照距离，从近到远
	for (int m = 0; m < L2.size(); m++) {
		for (int n = m + 1; n < L2.size(); n++) {
			if (L2.at(m) > L2.at(n)) {
				int t = L2.at(m);
				L2.at(m) = L2.at(n);
				L2.at(n) = t;
				KeyPoint kp = keypoint.at(m);
				keypoint.at(m) = keypoint.at(n);
				keypoint.at(n) = kp;
			}
		}
	}
	//去除边缘点
	vector<KeyPoint> new_keypoint;
	for (int i = 0; i < keypoint.size()-cut_number; i++) {
		new_keypoint.push_back(keypoint.at(i));
	}
	return new_keypoint;
}
vector<vector<KeyPoint>> FeatureExtract::ProcessFeatures(vector<vector<KeyPoint>> keypoints, int types, int nfeatures) {
	vector<vector<KeyPoint>> temp_keypoints;
	//vector<int> size;
	vector<vector<KeyPoint>>::iterator iter = keypoints.begin();
	//最小特征点个数
	int _min = keypoints.at(0).size();
	while (iter != keypoints.end()) {
		if (iter->size()<_min) {
			_min = iter->size();
		}
		iter++;
	}
	//计算每个图片的特征点重心，依据每个特征点到重心的距离对特征点排序,由近到远
	for (int i = 0; i < keypoints.size(); i++) {
		vector<double> L2;//距离
		double mid_x = 0.0;
		double mid_y = 0.0;
		for (int j = 0; j < keypoints.at(i).size(); j++) {
			mid_x += keypoints.at(i).at(j).pt.x;
			mid_y += keypoints.at(i).at(j).pt.y;
		}
		mid_x = mid_x / keypoints.at(i).size();
		mid_y = mid_y / keypoints.at(i).size();
		//距离计算
		for (int j = 0; j < keypoints.at(i).size(); j++) {
			L2.push_back(sqrt(pow((mid_x - keypoints.at(i).at(j).pt.x), 2) + pow((mid_y - keypoints.at(i).at(j).pt.y), 2)));
		}
		//排序，按照距离，从近到远
		for (int m = 0; m < L2.size(); m++) {
			for (int n = m + 1; n < L2.size(); n++) {
				if (L2.at(m) > L2.at(n)) {
					int t = L2.at(m);
					L2.at(m) = L2.at(n);
					L2.at(n) = t;
					KeyPoint kp = keypoints.at(i).at(m);
					keypoints.at(i).at(m) = keypoints.at(i).at(n);
					keypoints.at(i).at(n) = kp;
				}
			}
		}
		//cout << "特征个数：" << keypoints.at(i).size() << endl;
		//cout << "特征重心坐标：" << mid_x << " " << mid_y <<endl;
		//cout << "各点坐标" << endl;
		//for (int l = 0; l < keypoints.at(i).size(); l++) {
		//	cout << l<<":" << keypoints.at(i).at(l).pt.x << " " << keypoints.at(i).at(l).pt.y << ",距离重心距离：" << L2.at(l) << endl;
		//}
	}
	//if (nfeatures > _min) {
	//	nfeatures = _min;
	//}
	for (int i = 0; i < keypoints.size(); i++) {
		vector<KeyPoint> kp;
		if (keypoints.at(i).size() == 0) {
			for (int insert = i; insert > 0; insert--) {
				if (keypoints.at(insert).size() > 0) {
					keypoints.at(i) = keypoints.at(insert);
					break;
				}
			}
		}
		if (nfeatures <= keypoints.at(i).size()) {
			for (int j = 0; j < nfeatures; j++) {
				kp.push_back(keypoints.at(i).at(j));
			}
		}
		else {
			for (int j = 0; j < keypoints.at(i).size(); j++) {
				kp.push_back(keypoints.at(i).at(j));
			}
			for (int j = 0; j < nfeatures-keypoints.at(i).size(); j++) {
				kp.push_back(keypoints.at(i).at(0));
			}
		}
		if (kp.size() != nfeatures) {
			cout << "特征点数量不一致" << endl;
			exit(0);
		}
		temp_keypoints.push_back(kp);
	}
	return temp_keypoints;
}
Mat FeatureExtract::Hist(Mat input_image) {
	Mat image;//源图像  
	int hisSize[1], hisWidth, hisHeight;//直方图的大小,宽度和高度  
	float range[2];//直方图取值范围  
	const float *ranges;
	Mat channelsRGB[3];//分离的BGR通道  
	MatND outputRGB[3];//输出直方图分量  

	hisSize[0] = 256;
	hisWidth = 400;
	hisHeight = 400;
	range[0] = 0.0;
	range[1] = 255.0;
	ranges = &range[0];

	split(input_image, channelsRGB);
	calcHist(&channelsRGB[0], 1, 0, Mat(), outputRGB[0], 1, hisSize, &ranges);
	calcHist(&channelsRGB[1], 1, 0, Mat(), outputRGB[1], 1, hisSize, &ranges);
	calcHist(&channelsRGB[2], 1, 0, Mat(), outputRGB[2], 1, hisSize, &ranges);

	Mat rgbHist[3];
	for (int i = 0; i < 3; i++)
	{
		rgbHist[i] = Mat(hisWidth, hisHeight, CV_8UC3, Scalar::all(0));
	}
	normalize(outputRGB[0], outputRGB[0], 0, hisWidth - 20, NORM_MINMAX);
	normalize(outputRGB[1], outputRGB[1], 0, hisWidth - 20, NORM_MINMAX);
	normalize(outputRGB[2], outputRGB[2], 0, hisWidth - 20, NORM_MINMAX);
	for (int i = 0; i < hisSize[0]; i++)
	{
		int val = saturate_cast<int>(outputRGB[0].at<float>(i));
		rectangle(rgbHist[0], Point(i * 2 + 10, rgbHist[0].rows), Point((i + 1) * 2 + 10, rgbHist[0].rows - val), Scalar(0, 0, 255), 1, 8);
		val = saturate_cast<int>(outputRGB[1].at<float>(i));
		rectangle(rgbHist[1], Point(i * 2 + 10, rgbHist[1].rows), Point((i + 1) * 2 + 10, rgbHist[1].rows - val), Scalar(0, 255, 0), 1, 8);
		val = saturate_cast<int>(outputRGB[2].at<float>(i));
		rectangle(rgbHist[2], Point(i * 2 + 10, rgbHist[2].rows), Point((i + 1) * 2 + 10, rgbHist[2].rows - val), Scalar(255, 0, 0), 1, 8);
	}
	Mat result;
	vector<Mat> in;
	in.push_back(rgbHist[0]); in.push_back(rgbHist[1]); in.push_back(rgbHist[2]);
	vconcat(in,result);
	vector<Mat> __hist_feature;
	__hist_feature.push_back(outputRGB[0]); __hist_feature.push_back(outputRGB[1]); __hist_feature.push_back(outputRGB[2]);
	vconcat(__hist_feature, hist_feature);
	return result;
}
Mat FeatureExtract::GetHistFeature() {
	cout << "hist feature :" << hist_feature.rows << " " << hist_feature.cols << endl;
	//for (int i = 0; i < hist_feature.rows; i++) {
	//	uchar* data = hist_feature.ptr<uchar>(i);
	//	for (int j = 0; j < hist_feature.cols; j++) {
	//		cout <<(int) data[j] << " " << endl;
	//	}
	//	cout << endl;
	//}
	return hist_feature;
}
Mat FeatureExtract::HOG(Mat input_image) {
	HOGDescriptor hog;//使用的是默认的hog参数
	/*
	HOGDescriptor(Size win_size=Size(64, 128), Size block_size=Size(16, 16), Size block_stride=Size(8, 8),
	Size cell_size=Size(8, 8), int nbins=9, double win_sigma=DEFAULT_WIN_SIGMA(DEFAULT_WIN_SIGMA=-1),
	double threshold_L2hys=0.2, bool gamma_correction=true, int nlevels=DEFAULT_NLEVELS)
	Parameters:
	win_size – Detection window size. Align to block size and block stride.
	block_size – Block size in pixels. Align to cell size. Only (16,16) is supported for now.
	block_stride – Block stride. It must be a multiple of cell size.
	cell_size – Cell size. Only (8, 8) is supported for now.
	nbins – Number of bins. Only 9 bins per cell are supported for now.
	win_sigma – Gaussian smoothing window parameter.
	threshold_L2hys – L2-Hys normalization method shrinkage.
	gamma_correction – Flag to specify whether the gamma correction preprocessing is required or not.
	nlevels – Maximum number of detection window increases.
	*/
	//对于128*80的图片，blockstride = 8,15*9的block，2*2*9*15*9 = 4860
	//hog.cellSize = Size(4, 4);
	int width = 240;
	int height = width;
	hog.winSize = Size(width, height);
	hog.blockSize = Size(16, 16);
	//hog.cellSize = Size(4, 4);
	//hog.winSize = Size(input_image.cols, input_image.rows);
	vector<float> des;//HOG特征向量
	Mat dst;
	resize(input_image, dst, Size(width, height));//规范图像尺寸
	hog.compute(dst, des);//计算hog特征
	Mat background = Mat::zeros(Size(width, height), CV_8UC3);//设置黑色背景图，因为要用白色绘制hog特征
	Mat d = get_hogdescriptor_visual_image(background, des, hog.winSize, hog.cellSize, 2, 3.5);
	return d;
}
// adapted for arbitrary size of feature sets and training images
Mat FeatureExtract::get_hogdescriptor_visual_image(Mat& origImg,
	vector<float>& descriptorValues,//hog特征向量
	Size winSize,//图片窗口大小
	Size cellSize,
	int scaleFactor,//缩放背景图像的比例
	double viz_factor)//缩放hog特征的线长比例
{
	Mat visual_image;//最后可视化的图像大小
	cv::resize(origImg, visual_image, Size(origImg.cols*scaleFactor, origImg.rows*scaleFactor));
	int gradientBinSize = 9;
	// dividing 180° into 9 bins, how large (in rad) is one bin?
	float radRangeForOneBin = 3.14 / (float)gradientBinSize; //pi=3.14对应180°
    // prepare data structure: 9 orientation / gradient strenghts for each cell
	int cells_in_x_dir = winSize.width / cellSize.width;//x方向上的cell个数
	int cells_in_y_dir = winSize.height / cellSize.height;//y方向上的cell个数
	int totalnrofcells = cells_in_x_dir * cells_in_y_dir;//cell的总个数
	//注意此处三维数组的定义格式
	//int ***b;
	//int a[2][3][4];
	//int (*b)[3][4] = a;
	//gradientStrengths[cells_in_y_dir][cells_in_x_dir][9]
	float*** gradientStrengths = new float**[cells_in_y_dir];
	int** cellUpdateCounter = new int*[cells_in_y_dir];
	for (int y = 0; y < cells_in_y_dir; y++)
	{
		gradientStrengths[y] = new float*[cells_in_x_dir];
		cellUpdateCounter[y] = new int[cells_in_x_dir];
		for (int x = 0; x < cells_in_x_dir; x++)
		{
			gradientStrengths[y][x] = new float[gradientBinSize];
			cellUpdateCounter[y][x] = 0;
			for (int bin = 0; bin < gradientBinSize; bin++) {
				gradientStrengths[y][x][bin] = 0.0;//把每个cell的9个bin对应的梯度强度都初始化为0
			}
		}
	}
	// nr of blocks = nr of cells - 1
	// since there is a new block on each cell (overlapping blocks!) but the last one
	//相当于blockstride = (8,8)
	int blocks_in_x_dir = cells_in_x_dir - 1;
	int blocks_in_y_dir = cells_in_y_dir - 1;
	// compute gradient strengths per cell
	int descriptorDataIdx = 0;
	int cellx = 0;
	int celly = 0;
	for (int blockx = 0; blockx < blocks_in_x_dir; blockx++)
	{
		for (int blocky = 0; blocky < blocks_in_y_dir; blocky++)
		{
			// 4 cells per block ...
			for (int cellNr = 0; cellNr < 4; cellNr++)
			{
				// compute corresponding cell nr
				int cellx = blockx;
				int celly = blocky;
				if (cellNr == 1) celly++;
				if (cellNr == 2) cellx++;
				if (cellNr == 3)
				{
					cellx++;
					celly++;
				}
				for (int bin = 0; bin < gradientBinSize; bin++)
				{
					float gradientStrength = descriptorValues[descriptorDataIdx];
					descriptorDataIdx++;
					gradientStrengths[celly][cellx][bin] += gradientStrength;//因为C是按行存储
				} // for (all bins)
				  // note: overlapping blocks lead to multiple updates of this sum!
				  // we therefore keep track how often a cell was updated,
				  // to compute average gradient strengths
				cellUpdateCounter[celly][cellx]++;//由于block之间有重叠，所以要记录哪些cell被多次计算了
			} // for (all cells)


		} // for (all block x pos)
	} // for (all block y pos)
    // compute average gradient strengths
	for (int celly = 0; celly < cells_in_y_dir; celly++)
	{
		for (int cellx = 0; cellx < cells_in_x_dir; cellx++)
		{
			float NrUpdatesForThisCell = (float)cellUpdateCounter[celly][cellx];
			// compute average gradient strenghts for each gradient bin direction
			for (int bin = 0; bin < gradientBinSize; bin++)
			{
				gradientStrengths[celly][cellx][bin] /= NrUpdatesForThisCell;
			}
		}
	}
	cout << "winSize = " << winSize << endl;
	cout << "cellSize = " << cellSize << endl;
	cout << "blockSize = " << cellSize * 2 << endl;
	cout << "blockNum = " << blocks_in_x_dir << "×" << blocks_in_y_dir << endl;
	cout << "descriptorDataIdx = " << descriptorDataIdx << endl;
	// draw cells
	for (int celly = 0; celly < cells_in_y_dir; celly++)
	{
		for (int cellx = 0; cellx < cells_in_x_dir; cellx++)
		{
			int drawX = cellx * cellSize.width;
			int drawY = celly * cellSize.height;
			int mx = drawX + cellSize.width / 2;
			int my = drawY + cellSize.height / 2;
			cv::rectangle(visual_image,
				Point(drawX*scaleFactor, drawY*scaleFactor),
				Point((drawX + cellSize.width)*scaleFactor,
				(drawY + cellSize.height)*scaleFactor),
				CV_RGB(0, 0, 0),//cell框线的颜色
				1);
			// draw in each cell all 9 gradient strengths
			for (int bin = 0; bin < gradientBinSize; bin++)
			{
				float currentGradStrength = gradientStrengths[celly][cellx][bin];
				// no line to draw?
				if (currentGradStrength == 0)
					continue;
				float currRad = bin * radRangeForOneBin + radRangeForOneBin / 2;//取每个bin里的中间值，如10°,30°,...,170°.
				float dirVecX = cos(currRad);
				float dirVecY = sin(currRad);
				float maxVecLen = cellSize.width / 2;
				float scale = viz_factor; // just a visual_imagealization scale,
				// to see the lines better
				// compute line coordinates
				float x1 = mx - dirVecX * currentGradStrength * maxVecLen * scale;
				float y1 = my - dirVecY * currentGradStrength * maxVecLen * scale;
				float x2 = mx + dirVecX * currentGradStrength * maxVecLen * scale;
				float y2 = my + dirVecY * currentGradStrength * maxVecLen * scale;
				// draw gradient visual_imagealization
				cv::line(visual_image,
					Point(x1*scaleFactor, y1*scaleFactor),
					Point(x2*scaleFactor, y2*scaleFactor),
					CV_RGB(255, 255,255),//HOG可视化的cell的颜色
					1);
			} // for (all bins)
		} // for (cellx)
	} // for (celly)
	// don't forget to free memory allocated by helper data structures!
	for (int y = 0; y < cells_in_y_dir; y++)
	{
		for (int x = 0; x < cells_in_x_dir; x++)
		{
			delete[] gradientStrengths[y][x];
		}
		delete[] gradientStrengths[y];
		delete[] cellUpdateCounter[y];
	}
	delete[] gradientStrengths;
	delete[] cellUpdateCounter;
	return visual_image;//返回最终的HOG可视化图像
}
Mat FeatureExtract::GetDescriptor() {
	return descriptor;
}
vector<vector<KeyPoint>> FeatureExtract::GetFeatures() {
	return feature_keypoints;
}
//lbp--------------------------------------------------------------------------------
Mat FeatureExtract::LBP(Mat input_image, int lbp_type) {
	Mat result;
	switch (lbp_type)
	{
	case LBP_C:
		result = lbps(input_image);
		break;
	case LBP_M:
		result = lbpm(input_image);
		break;
	case LBP_M_LOCAL:
		result = lbpm_local(input_image,2,8);
		break;
	case LBP_C_LOCAL_WHOLE:
		result = lbpc_local_whole(input_image);
		break;
	case LBP_S:
		result = lbps(input_image);
		break;
	case LBP_M_LOCAL_WHOLE:
		result = lbpm_local_whole(input_image);
		break;
	case LBPC_LOCAL:
		result = lbpc_local(input_image);
		break;
	default:
		result = lbps(input_image);
		break;
	}
	return result;
}
Mat FeatureExtract::LBP(Mat input_image, int lbp_type , int raduis , int neighbors) {
	Mat result;
	switch (lbp_type)
	{
	case LBP_C:
		result = lbps(input_image,raduis,neighbors);
		break;
	case LBP_M:
		result = lbpm(input_image, raduis, neighbors);
		break;
	case LBP_M_LOCAL:
		result = lbpm_local(input_image, raduis, neighbors);
		break;
	case LBP_C_LOCAL_WHOLE:
		result = lbpc_local_whole(input_image, raduis, neighbors);
		break;
	case LBP_S:
		result = lbps(input_image, raduis, neighbors);
		break;
	case LBP_M_LOCAL_WHOLE:
		result = lbpm_local_whole(input_image, raduis, neighbors);
		break;
	case LBPC_LOCAL:
		result = lbpc_local(input_image, raduis, neighbors);
		break;
	default:
		result = lbps(input_image, raduis, neighbors);
		break;
	}
	return result;
}
double FeatureExtract::global_average_difference(const cv::Mat& src, int radius , int neighbors ) {
	assert(radius > 0 && neighbors > 0);
	//compute global average difference
	double averageDiff = 0;
	for (int n = 0; n < neighbors; n++) {
		// position of sample point
		float x = static_cast<float>(-radius * sin(2.0*CV_PI*n / static_cast<float>(neighbors)));
		float y = static_cast<float>(radius * cos(2.0*CV_PI*n / static_cast<float>(neighbors)));

		// flooring and ceiling
		int fx = static_cast<int>(floor(x));
		int fy = static_cast<int>(floor(y));
		int cx = static_cast<int>(ceil(x));
		int cy = static_cast<int>(ceil(y));

		// decimals
		float ty = y - fy;
		float tx = x - fx;

		// weight of interpolation
		float w1 = (1 - tx) * (1 - ty);
		float w2 = tx  * (1 - ty);
		float w3 = (1 - tx) *      ty;
		float w4 = tx  *      ty;

		// processing image data
		for (int i = radius; i < src.rows - radius; i++) {
			for (int j = radius; j < src.cols - radius; j++) {
				// compute interpolation value
				float t = static_cast<float>(w1*src.at<uchar>(i + fy, j + fx) + w2*src.at<uchar>(i + fy, j + cx)
					+ w3*src.at<uchar>(i + cy, j + fx) + w4*src.at<uchar>(i + cy, j + cx));
				averageDiff += abs(t - src.at<uchar>(i, j));
			}
		}//processing data for

	}//outer  for
	averageDiff /= (neighbors*(src.rows - radius)*(src.cols - radius));

	return averageDiff;
}
double FeatureExtract::local_average_difference(const cv::Mat& src, int i, int j, int radius , int neighbors ) {
	assert(i >= 0 && i < src.cols && j >= 0 && j < src.rows);
	assert(radius > 0 && neighbors > 0);

	double averageDiff = 0;
	for (int n = 0; n < neighbors; n++) {
		// compute position of sample point
		float x = static_cast<float>(-radius * sin(2.0*CV_PI*n / static_cast<float>(neighbors)));
		float y = static_cast<float>(radius * cos(2.0*CV_PI*n / static_cast<float>(neighbors)));

		// flooring and ceiling
		int fx = static_cast<int>(floor(x));
		int fy = static_cast<int>(floor(y));
		int cx = static_cast<int>(ceil(x));
		int cy = static_cast<int>(ceil(y));

		// decimals 
		float ty = y - fy;
		float tx = x - fx;

		// weight of interpolation
		float w1 = (1 - tx) * (1 - ty);
		float w2 = tx  * (1 - ty);
		float w3 = (1 - tx) *      ty;
		float w4 = tx  *      ty;

		// sum of interpolation value
		float t = static_cast<float>(w1*src.at<uchar>(i + fy, j + fx) + w2*src.at<uchar>(i + fy, j + cx)
			+ w3*src.at<uchar>(i + cy, j + fx) + w4*src.at<uchar>(i + cy, j + cx));
		averageDiff += abs(t - src.at<uchar>(i, j));
	}
	averageDiff /= neighbors;

	return averageDiff;
}
double FeatureExtract::global_average_gray(const cv::Mat& src) {
	double whole = 0;
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			whole += src.at<uchar>(i, j);
		}
	}
	whole /= (src.rows)*(src.cols);
	return whole;
}
double FeatureExtract::local_average_gray(const cv::Mat& src, int i, int j, int radius , int neighbors ) {
	assert(i >= 0 && i < src.cols && j >= 0 && j < src.rows);
	assert(radius > 0 && neighbors > 0);

	double t = 0;
	for (int n = 0; n < neighbors; n++) {
		// compute position of sample point
		float x = static_cast<float>(-radius * sin(2.0*CV_PI*n / static_cast<float>(neighbors)));
		float y = static_cast<float>(radius * cos(2.0*CV_PI*n / static_cast<float>(neighbors)));

		// flooring ceiling
		int fx = static_cast<int>(floor(x));
		int fy = static_cast<int>(floor(y));
		int cx = static_cast<int>(ceil(x));
		int cy = static_cast<int>(ceil(y));

		// decimals 
		float ty = y - fy;
		float tx = x - fx;

		// weight of interpolation
		float w1 = (1 - tx) * (1 - ty);
		float w2 = tx  * (1 - ty);
		float w3 = (1 - tx) *      ty;
		float w4 = tx  *      ty;

		// sum of interpolation value
		t += static_cast<float>(w1*src.at<uchar>(i + fy, j + fx) + w2*src.at<uchar>(i + fy, j + cx)
			+ w3*src.at<uchar>(i + cy, j + fx) + w4*src.at<uchar>(i + cy, j + cx));
	}

	t /= neighbors;

	return t;
}
//local gray difference code
Mat FeatureExtract::lbps(const cv::Mat& src, int radius , int neighbors ) {
	assert(src.data);
	assert(radius > 0 && neighbors > 0);
	assert((src.type() == CV_8UC1));
	assert(radius > 0 && neighbors > 0);

	cv::Mat dst(src.rows, src.cols, CV_8UC1);
	dst.setTo(0);

	for (int n = 0; n < neighbors; n++) {

		// position of sample point
		float x = static_cast<float>(-radius * sin(2.0*CV_PI*n / static_cast<float>(neighbors)));
		float y = static_cast<float>(radius * cos(2.0*CV_PI*n / static_cast<float>(neighbors)));

		// flooring and ceiling
		int fx = static_cast<int>(floor(x));
		int fy = static_cast<int>(floor(y));
		int cx = static_cast<int>(ceil(x));
		int cy = static_cast<int>(ceil(y));

		// decimals
		float ty = y - fy;
		float tx = x - fx;

		// weight of interpolation
		float w1 = (1 - tx) * (1 - ty);
		float w2 = tx* (1 - ty);
		float w3 = (1 - tx) *ty;
		float w4 = tx*ty;

		// processing image data
		for (int i = radius; i < src.rows - radius; i++) {
			for (int j = radius; j < src.cols - radius; j++) {
				// compute interpolation value
				float t = static_cast<float>(w1*src.at<uchar>(i + fy, j + fx) + w2*src.at<uchar>(i + fy, j + cx)
					+ w3*src.at<uchar>(i + cy, j + fx) + w4*src.at<uchar>(i + cy, j + cx));
				// coding
				dst.at<uchar>(i - radius, j - radius) += ((t > src.at<uchar>(i, j)) ||
					(std::abs(t - src.at<uchar>(i, j)) < std::numeric_limits<float>::epsilon())) << n;
			}
		}//processing data for

	}//outer  for

	return dst;
}
//differnece between local differnce and global average differnce code
Mat FeatureExtract::lbpm(const cv::Mat& src, int radius , int neighbors ) {
	assert(src.data);
	assert(radius > 0 && neighbors > 0);
	assert((src.type() == CV_8UC1));
	assert(radius > 0 && neighbors > 0);

	cv::Mat dst(src.rows, src.cols, CV_8UC1);
	dst.setTo(0);

	double averageDiff = global_average_difference(src, radius, neighbors);

	for (int n = 0; n < neighbors; n++) {

		// position of sample point
		float x = static_cast<float>(-radius * sin(2.0*CV_PI*n / static_cast<float>(neighbors)));
		float y = static_cast<float>(radius * cos(2.0*CV_PI*n / static_cast<float>(neighbors)));

		// flooring and ceiling
		int fx = static_cast<int>(floor(x));
		int fy = static_cast<int>(floor(y));
		int cx = static_cast<int>(ceil(x));
		int cy = static_cast<int>(ceil(y));

		// decimals
		float ty = y - fy;
		float tx = x - fx;

		// weight of interpolation
		float w1 = (1 - tx) * (1 - ty);
		float w2 = tx  * (1 - ty);
		float w3 = (1 - tx) *      ty;
		float w4 = tx  *      ty;

		// processing image data
		for (int i = radius; i < src.rows - radius; i++) {
			for (int j = radius; j < src.cols - radius; j++) {
				// compute interpolation value
				float t = static_cast<float>(w1*src.at<uchar>(i + fy, j + fx) + w2*src.at<uchar>(i + fy, j + cx)
					+ w3*src.at<uchar>(i + cy, j + fx) + w4*src.at<uchar>(i + cy, j + cx));

				// coding
				t -= src.at<uchar>(i, j);
				dst.at<uchar>(i - radius, j - radius) += ((abs(t) > averageDiff) ||
					(std::abs(abs(t) - averageDiff) < std::numeric_limits<float>::epsilon())) << n;

			}
		}//processing data for
	}//outer  for

	return dst;
}
//differnece between local differnce and local average differnce code
Mat FeatureExtract::lbpm_local(const cv::Mat& src, int radius , int neighbors ) {
	assert(src.data);
	assert(radius > 0 && neighbors > 0);
	assert((src.type() == CV_8UC1));
	assert(radius > 0 && neighbors > 0);

	cv::Mat dst(src.rows, src.cols, CV_8UC1);
	dst.setTo(0);

	for (int i = radius; i < src.rows - radius; i++) {
		for (int j = radius; j < src.cols - radius; j++) {
			double averageDiff = local_average_difference(src, i, j, radius, neighbors);
			for (int n = 0; n < neighbors; n++) {
				// compute position of sample point
				float x = static_cast<float>(-radius * sin(2.0*CV_PI*n / static_cast<float>(neighbors)));
				float y = static_cast<float>(radius * cos(2.0*CV_PI*n / static_cast<float>(neighbors)));

				// flooring and ceiling
				int fx = static_cast<int>(floor(x));
				int fy = static_cast<int>(floor(y));
				int cx = static_cast<int>(ceil(x));
				int cy = static_cast<int>(ceil(y));

				// decimals 
				float ty = y - fy;
				float tx = x - fx;

				// weight of interpolation
				float w1 = (1 - tx) * (1 - ty);
				float w2 = tx  * (1 - ty);
				float w3 = (1 - tx) *      ty;
				float w4 = tx  *      ty;

				// sum of interpolation value
				float t = static_cast<float>(w1*src.at<uchar>(i + fy, j + fx) + w2*src.at<uchar>(i + fy, j + cx)
					+ w3*src.at<uchar>(i + cy, j + fx) + w4*src.at<uchar>(i + cy, j + cx));

				// coding
				t -= src.at<uchar>(i, j);
				dst.at<uchar>(i - radius, j - radius) += ((abs(t) > averageDiff) ||
					(std::abs(abs(t) - averageDiff) < std::numeric_limits<float>::epsilon())) << n;
			}

		}//for
	}//for

	return dst;

}
//differnece between local average differnce and local average differnce
Mat FeatureExtract::lbpm_local_whole(const cv::Mat& src, int radius , int neighbors ) {
	assert(src.data);
	assert(radius > 0 && neighbors > 0);
	assert((src.type() == CV_8UC1));
	assert(radius > 0 && neighbors > 0);

	cv::Mat dst(src.rows, src.cols, CV_8UC1);
	dst.setTo(0);

	double global = global_average_difference(src, radius, neighbors);
	for (int i = radius; i < src.rows - radius; i++) {
		for (int j = radius; j < src.cols - radius; j++) {
			double local = local_average_difference(src, i, j, radius, neighbors);
			if (local > global) {
				dst.at<uchar>(i, j) = 255;
			}
			else {
				dst.at<uchar>(i, j) = 0;
			}
		}
	}
	return dst;
}
//diffrence between local gray and global average gray  
Mat FeatureExtract::lbpc(const cv::Mat& src) {
	assert(src.data);
	assert((src.type() == CV_8UC1));

	cv::Mat dst(src.rows, src.cols, CV_8UC1);
	dst.setTo(0);

	double whole = global_average_gray(src);

	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			if (src.at<uchar>(i, j) > whole) {
				dst.at<uchar>(i, j) = 255;
			}
			else {
				dst.at<uchar>(i, j) = 0;
			}

		}
	}
	return dst;
}
//difference between local gray and local average gray  
Mat FeatureExtract::lbpc_local(const cv::Mat& src, int radius , int neighbors ) {
	assert(src.data);
	assert(radius > 0 && neighbors > 0);
	assert((src.type() == CV_8UC1));
	cv::Mat dst(src.rows, src.cols, CV_8UC1);
	dst.setTo(0);
	for (int i = radius; i < src.rows - radius; i++) {
		for (int j = radius; j < src.cols - radius; j++) {
			double local = local_average_gray(src, i, j, radius, neighbors);
			if (src.at<uchar>(i, j) > local) {
				dst.at<uchar>(i - radius, j - radius) = 255;
			}
			else {
				dst.at<uchar>(i - radius, j - radius) = 0;
			}
		}
	}
	return dst;
}
//difference between local average gray and local average gray 
Mat FeatureExtract::lbpc_local_whole(const cv::Mat& src, int radius , int neighbors ) {
	assert(src.data);
	assert(radius > 0 && neighbors > 0);
	assert((src.type() == CV_8UC1));
	assert(radius > 0 && neighbors > 0);

	cv::Mat dst(src.rows, src.cols, CV_8UC1);
	dst.setTo(0);

	double whole = global_average_gray(src);

	for (int i = radius; i < src.rows - radius; i++) {
		for (int j = radius; j < src.cols - radius; j++) {

			double local = local_average_gray(src, i, j, radius, neighbors);
			if (local > whole) {
				dst.at<uchar>(i - radius, j - radius) = 255;
			}
			else {
				dst.at<uchar>(i - radius, j - radius) = 0;
			}
		}
	}
	return dst;
}