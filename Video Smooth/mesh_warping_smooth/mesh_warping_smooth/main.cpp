#include <iostream>
#include "commonio.h"
#include"union_smooth.h"
//#include"Motion_conformance.h"
#include"Median_Filter.h"
#include <direct.h>
#include <iomanip>
#include <process.h>  
#include <windows.h>  
#include<io.h>
using namespace std;
using namespace cv;
using namespace Bundled;

const double pi = 3.1415926;
const double e = 2.718282;

inline double computer_sigma(double value)
{
	double sigma = sqrt(-(value*value) / (2 * log(0.5)));
	return sigma;
}

void save_info(double smooth, double warping, string video,string file)
{
	ofstream write;
	write.open(file,ios::app);

	string smooth_str;
	string warping_str;
	stringstream change;
	change << smooth <<" "<< warping;
	change >> smooth_str >> warping_str;
	
	write << endl << video +":"<< "   " << smooth_str << "   " << warping_str << endl;
	write.close();
	
}

void run_with_different_parmiter(string video_name, string video_file, string path_file, string save_path,double* parmeter)
{
	VideoCapture vc(video_file + video_name + ".mp4");
	int frame_num = (int)(vc.get(CV_CAP_PROP_FRAME_COUNT));
	int frame_w = (int)(vc.get(CV_CAP_PROP_FRAME_WIDTH));
	int frame_h = (int)(vc.get(CV_CAP_PROP_FRAME_HEIGHT));
	double Fps = (double)(vc.get(CV_CAP_PROP_FPS));



	vector<vector<v2d>> frame_points;
	set_of_path org_paths;

	double mesh_w = 40;
	double mesh_h = 40;

	vector<std::vector<int>> index;
	vector<vector<Point2i>> index_frame_point;
	int wigth = floor(frame_w / mesh_w);
	int height = floor(frame_h / mesh_h);


	//read path
	Mat all_path = read_path_and_save_in_mat(path_file + video_name + ".mp4_path.txt", org_paths, frame_num);
	ComputePointsOfFrame(frame_num, frame_points, index, org_paths);

	cout << "Start smooth" << endl;



	Bundled::union_smooth smooth(video_file + video_name + ".mp4",
		save_path + video_name +"_"+ to_string((int)parmeter[0]) + "_" + to_string((int)parmeter[1])+"_"+ to_string((int)parmeter[2])+"_"+ to_string((int)parmeter[3]) + "_" + to_string((int)parmeter[4]) + "_.avi",
		save_path + video_name +"_" + to_string((int)parmeter[0]) + "_" + to_string((int)parmeter[1]) + "_" + to_string((int)parmeter[2]) + "_" + to_string((int)parmeter[3]) + "_" + to_string((int)parmeter[4]) + "_mesh.mp4",
		org_paths, index, frame_h, frame_w, mesh_w, mesh_h, parmeter[0], parmeter[1], parmeter[2], parmeter[3], parmeter[4]);


	double t1 = cv::getTickCount();


	//set control points  
	smooth.set_control_points(frame_points);

	//set weigh for all mesh vertex  
	smooth.ComputeMeshPointsAndWeight(frame_points);

	//build optimization equation and solve by eigen
	smooth.solve_by_Eigen(false);

	cout << "Smooth over" << endl;

	double t2 = cv::getTickCount();
	cout << "video:" << video_name << "smooth cost time is" << (t2 - t1) / cv::getTickFrequency() << endl;

	//warping

	t1 = cv::getTickCount();
	smooth.change_mesh();
	smooth.find_homograph_from_warped_to_org();

	smooth.warping_video(Fps,false);
	t2 = cv::getTickCount();
	cout << "video:" << video_name << "warping cost time is" << (t2 - t1) / cv::getTickFrequency() << endl;

	vector<vector<v2d>>(frame_points).swap(frame_points);
	set_of_path(org_paths).swap(org_paths);
	vector<std::vector<int>>(index).swap(index);
	vector<vector<Point2i>>(index_frame_point).swap(index_frame_point);
}


void getAllFiles(string path, vector<string>& files, string format)
{
	long long hFile = 0;//文件句柄  64位下long 改为 intptr_t
	struct _finddata_t fileinfo;//文件信息 
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*" + format).c_str(), &fileinfo)) != -1) //文件存在
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))//判断是否为文件夹
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)//文件夹名中不含"."和".."
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name)); //保存文件夹名
					//getAllFiles(p.assign(path).append("\\").append(fileinfo.name), files, format); //递归遍历文件夹
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));//如果不是文件夹，储存文件名
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

int main()
{
	vector<string>class_name{"Regular","Parallax","Crowd"};
	for (int i = 1; i < class_name.size(); i++)
	{
		string video_file = "E:\\data\\TVCG\\unstable\\" + class_name[i]+ "\\videos\\";
		string path_file = "E:\\data\\TVCG\\unstable\\" + class_name[i] + "\\paths1000\\";
		string save_path = "E:\\data\\TVCG\\results\\" + class_name[i] + "\\";
		string format = ".txt";
		vector<string>files_unstable;
		getAllFiles(path_file, files_unstable, format);

		for (int j = 0; j < files_unstable.size();j++)
		{ 
			
			int start_num = files_unstable[j].find_last_of("\\");
			string video_name = files_unstable[j].substr(start_num+1,files_unstable[j].length()-1-start_num-4-5-4);

			double alpha_1 = 20;
			double alpha_2 = 10;
			double Beta_1 = 10;
			double Beta_2 = 80;
			double gamma = 0.01;
			double parmeter[5] = { alpha_1, alpha_2, Beta_1, Beta_2, gamma };

			run_with_different_parmiter(video_name, video_file, path_file, save_path, parmeter);
		}
	}
}




