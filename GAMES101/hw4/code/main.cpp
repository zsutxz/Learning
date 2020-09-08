#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

std::vector<cv::Point2f> control_points;

void mouse_handler(int event, int x, int y, int flags, void *userdata) 
{
    if (event == cv::EVENT_LBUTTONDOWN && control_points.size() < 4) 
    {
        std::cout << "Left button of the mouse is clicked - position (" << x << ", "
        << y << ")" << '\n';
        control_points.emplace_back(x, y);
    }     
}

void naive_bezier(const std::vector<cv::Point2f> &points, cv::Mat &window) 
{
    auto &p_0 = points[0];
    auto &p_1 = points[1];
    auto &p_2 = points[2];
    auto &p_3 = points[3];

    for (double t = 0.0; t <= 1.0; t += 0.001) 
    {
        auto point = std::pow(1 - t, 3) * p_0 + 3 * t * std::pow(1 - t, 2) * p_1 +
                 3 * std::pow(t, 2) * (1 - t) * p_2 + std::pow(t, 3) * p_3;

        window.at<cv::Vec3b>(point.y, point.x)[2] = 255;
    }
}

int factorial(int x)
{
    int f;

    if (x == 0 || x == 1)
        f = 1;
    else
        f = factorial(x - 1) * x;

    return f;
}


cv::Point2f recursive_bezier(const std::vector<cv::Point2f> &control_points, float t) 
{
    // TODO: Implement de Casteljau's algorithm

    cv::Point2f  point = {0.5f,0.5f};

    int n = control_points.size(); 
    // //数学方法
    // for(int i = 0;i<n;i++)
    // {
    //     int c = factorial(n-1)/(factorial(i)*factorial(n-1-i));
    //     point += c*control_points[i]*std::pow(1-t,n-1-i)*std::pow(t,i);
    // }

    //递归方法
    if(n==1)
    {
        point += control_points[0];
    }
    else
    {
        cv::Point2f left_bezier_point, right_bezier_point;
        
        std::vector<cv::Point2f> left_points(&(control_points[0]), &(control_points[n-1]));
        std::vector<cv::Point2f> right_points(&(control_points[1]), &(control_points[n]));
        
        std::cout<<"n:"<<n<<std::endl;
        std::cout<<"control_points:"<<control_points<<std::endl;
        std::cout<<"left_point:"<<left_points<<std::endl;
        std::cout<<"right_point:"<<right_points<<std::endl;
        
        // std::vector<cv::Point2f> left_points(control_points.begin(),control_points.end()-1);
        // std::vector<cv::Point2f> right_points(control_points.begin()+1,control_points.end());
        left_bezier_point = recursive_bezier(left_points,t);    
        right_bezier_point = recursive_bezier(right_points,t); 
        point =(1-t)*left_bezier_point+t*right_bezier_point;
    
    }
    

    return point;

}


void bezier(const std::vector<cv::Point2f> &control_points, cv::Mat &window) 
{
    // TODO: Iterate through all t = 0 to t = 1 with small steps, and call de Casteljau's 
    // recursive Bezier algorithm.

    for (double t = 0.0; t <= 1.0; t += 0.001) 
    {
        cv::Point2f point = recursive_bezier(control_points,t);

        window.at<cv::Vec3b>(point.y, point.x)[0] = 255;
    }
}

int main() 
{
    cv::Mat window = cv::Mat(700, 700, CV_8UC3, cv::Scalar(0));
    cv::cvtColor(window, window, cv::COLOR_BGR2RGB);
    cv::namedWindow("Bezier Curve", cv::WINDOW_AUTOSIZE);

    cv::setMouseCallback("Bezier Curve", mouse_handler, nullptr);

    int key = -1;
    while (key != 27) 
    {
        for (auto &point : control_points) 
        {
            cv::circle(window, point, 3, {255, 255, 255}, 3);
        }

        if (control_points.size() == 3) 
        {
            // naive_bezier(control_points, window);
            bezier(control_points, window);

            cv::imshow("Bezier Curve", window);
            cv::imwrite("my_bezier_curve.png", window);
            key = cv::waitKey(0);

            return 0;
        }

        cv::imshow("Bezier Curve", window);
        key = cv::waitKey(20);
    }

return 0;
}
