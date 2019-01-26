#define WIN32_LEAN_AND_MEAN

#include "Windows.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "bmp/EasyBMP.h"

using namespace std;

const int whiteThreshold = 200;

string sourceName = "img.bmp";
const string kOutputName = "gradient";

const int tickCount = GetTickCount64();

void GetGradientValues(BMP& profile, vector<int>& list)
{
  for (int x = 0; x < profile.TellWidth(); ++x)
  {
    int depth = 0;

    for (int y = 0; y < profile.TellHeight(); ++y)
    {
      const RGBApixel p = profile.GetPixel(x, y);
      if (((p.Red+p.Blue+p.Green) / 3) < whiteThreshold)
      {
        break;
      }

      ++depth;
    }

    const float gradientVal = (1-(static_cast<float>(depth) / profile.TellHeight())) * 255;
    list.push_back(gradientVal);
  }
}

void PaintGradientToCanvas(BMP& canvas, vector<int>& list)
{
  for (int x = 0; x < canvas.TellWidth(); ++x)
  {
    const RGBApixel p = { list.at(x), list.at(x), list.at(x), 0 };
    for (int y = 0; y < canvas.TellHeight(); ++y)
    {
      canvas.SetPixel(x, y, p);
    }
  }
}

void ShowError()
{
  wstring msg = L"Could not find : ";
  msg += wstring(sourceName.begin(), sourceName.end());

  MessageBox(
    nullptr,
    msg.c_str(),
    L"Error",
    MB_ICONEXCLAMATION
  );
}

int main(const int argc, char* argv[])
{
  //AL.
  //For debugging!
  #ifdef _DEBUG
  MessageBox(nullptr, L"Attach", L"", 0);
  #endif
  for (int i = 0; i < argc; ++i)
  {
    cout << argv[i] << "\n";
  }
  cout << "\nCreating Gradient Reference...";
  //

  if (argc > 1)
  {
    sourceName = argv[1];
  }

  BMP profile;
  if (profile.ReadFromFile(sourceName.c_str()) == false)
  {
    ShowError();
    return -1;
  }

  vector<int> list;
  GetGradientValues(profile, list);

  BMP canvas;
  canvas.SetSize(profile.TellWidth(), profile.TellHeight());
  PaintGradientToCanvas(canvas, list);

  const string tickString = to_string(tickCount);

  CreateDirectoryA(tickString.c_str(), nullptr);

  const string outputName_image = tickString + "/" + kOutputName + ".bmp";
  canvas.WriteToFile(outputName_image.c_str());

  const string outputName_text = tickString + "/" + kOutputName + ".txt";
  ofstream outFile(outputName_text);
  for (const auto &e : list)
  {
    outFile << e << "\n";
  }

  return tickCount;
}
