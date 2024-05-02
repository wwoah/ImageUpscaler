//CSCI-551-Spring 2024
//Parallel Project
//Image upscaler using Lanczos Kernel and Interpolation
//Created by Phillip Bishop
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <time.h>

using namespace std;

// Lanczos kernel function
double lanczos(double x, double a) {
    double pi = 3.14159265358979323846;
    if (x == 0) { return 1.0; }
    else if (x > -a && x < a) { return a * sin(pi * x) * sin(pi * x / a) / (pi * pi * x * x); }
    else { return 0.0; }
}

int main(int argc, char* argv[]) {

    //Timer variables
    struct timespec start, end;
    double fstart, fend;

    //Open input file
    ifstream input(argv[1], ios::binary);
    if (!input) {
        cerr << "Error opening input file" << endl;
        return 1;
    }

    //Read PGM header
    string pval;
    int width, height, max_value;
    input >> pval >> width >> height >> max_value;
    if (pval != "P5" || max_value != 255) {
        cerr << "Error: Input file is not a grayscale P5 PGM file" << endl;
        return 1;
    }

    //Read input file data
    vector<unsigned char> input_pixels(width * height);
    input.read(reinterpret_cast<char*>(input_pixels.data()), input_pixels.size());
    input.close();

    //Upscale image using Lanczos interpolation
    const int output_width = 1920;
    const int output_height = 1080;
    vector<unsigned char> output_pixels(output_width * output_height, 0);

    const double scale_x = static_cast<double>(width) / output_width;
    const double scale_y = static_cast<double>(height) / output_height;
    const double radius = 2.0;
   
    //Start timer
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int y = 0; y < output_height; y++) {
        for (int x = 0; x < output_width; x++) {
            double sum = 0.0;
            double weight_sum = 0.0;

            for (int j = max(0, static_cast<int>(floor(y * scale_y - radius + 0.5))); j < min(height, static_cast<int>(ceil(y * scale_y + radius + 0.5))); j++) {
                for (int i = max(0, static_cast<int>(floor(x * scale_x - radius + 0.5))); i < min(width, static_cast<int>(ceil(x * scale_x + radius + 0.5))); i++) {
                    double weight = lanczos(x * scale_x - i, radius) * lanczos(y * scale_y - j, radius);
                    sum += weight * input_pixels[j * width + i];
                    weight_sum += weight;
                }
            }

            if (weight_sum > 0) {
                sum /= weight_sum;
            }

            output_pixels[y * output_width + x] = static_cast<unsigned char>(sum);
        }
    }

    //Write output file
    ofstream output("output.pgm", ios::binary);
    if (!output) {
        cerr << "Error opening output file" << endl;
        return 1;
    }

    output << "P5" << endl << output_width << " " << output_height << endl << "255" << endl;
    output.write(reinterpret_cast<char*>(output_pixels.data()), output_pixels.size());
    output.close();

    //Notify User
    cout << "Image upscaled successfully" << endl;
    
    //Display elapsed time
    clock_gettime(CLOCK_MONOTONIC, &end);
    fstart=start.tv_sec + (start.tv_nsec / 1000000000.0);
    fend=end.tv_sec + (end.tv_nsec / 1000000000.0);

    printf("Elapsed Time: %lf\n", (fend-fstart));

    return 0;
}
