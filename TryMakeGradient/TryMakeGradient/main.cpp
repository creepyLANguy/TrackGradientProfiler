#define WIN32_LEAN_AND_MEAN

#include <opencv2/highgui/highgui.hpp>
#include "Windows.h"
#include <fstream>

using namespace cv;
using namespace std;

const int whiteThreshold = 200;

const string kSourceName = "img.bmp";
const string kOutputName = "gradient";


void GetGradientValues(const Mat& profile, vector<int>& list)
{
  for (int x = 0; x < profile.cols; ++x)
  {
    int depth = 0;

    for (int y = 0; y < profile.rows; ++y)
    {
      const uchar px = profile.at<uchar>(y, x);
      if (px < whiteThreshold)
      {
        break;
      }

      ++depth;
    }

    const float gradientVal = (1-(static_cast<float>(depth) / profile.rows)) * 255;
    list.push_back(gradientVal);
  }
}

void PaintGradientToCanvas(Mat* canvas, vector<int>& list)
{
  for (int x = 0; x < canvas->cols; ++x)
  {
    const int val = list.at(x);
    for (int y = 0; y < canvas->rows; ++y)
    {
      canvas->at<uchar>(y, x) = val;
    }
  }
}

void ShowError()
{
  wstring msg = L"Could not find : ";
  msg += wstring(kSourceName.begin(), kSourceName.end());

  MessageBox(
    nullptr,
    msg.c_str(),
    L"Error",
    MB_ICONEXCLAMATION
  );
}

void main()
{
  const Mat profile = imread(kSourceName, CV_LOAD_IMAGE_GRAYSCALE);

  if (profile.data == nullptr)
  {
    ShowError();
    return;
  }

  vector<int> list;
  GetGradientValues(profile, list);

  Mat* canvas = new Mat(profile.rows, profile.cols, CV_LOAD_IMAGE_GRAYSCALE);
  PaintGradientToCanvas(canvas, list);

  const string tickString = to_string(GetTickCount64());

  CreateDirectoryA(tickString.c_str(), nullptr);

  const string outputName_image = tickString + "/" + kOutputName + ".bmp";
  imwrite(outputName_image, *canvas);

  const string outputName_text = tickString + "/" + kOutputName + ".txt";
  ofstream outFile(outputName_text);
  for (const auto &e : list)
  {
    outFile << e << "\n";
  }
}
