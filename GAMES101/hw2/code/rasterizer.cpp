// clang-format off
//
// Created by goksu on 4/6/19.
//

#include <algorithm>
#include <vector>
#include "rasterizer.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>

rst::pos_buf_id rst::rasterizer::load_positions(const std::vector<Eigen::Vector3f> &positions)
{
    auto id = get_next_id();
    pos_buf.emplace(id, positions);

    return {id};
}

rst::ind_buf_id rst::rasterizer::load_indices(const std::vector<Eigen::Vector3i> &indices)
{
    auto id = get_next_id();
    ind_buf.emplace(id, indices);

    return {id};
}

rst::col_buf_id rst::rasterizer::load_colors(const std::vector<Eigen::Vector3f> &cols)
{
    auto id = get_next_id();
    col_buf.emplace(id, cols);

    return {id};
}

auto to_vec4(const Eigen::Vector3f& v3, float w = 1.0f)
{
    return Vector4f(v3.x(), v3.y(), v3.z(), w);
}


static bool insideTriangle(double x, double y, const Vector3f* _v)
{   
    // TODO : Implement this function to check if the point (x, y) is inside the triangle represented by _v[0], _v[1], _v[2]
    
    // Eigen::Vector2f p;
    // p<<x,y;
    // Eigen::Vector2f AB = _v[1].head(2) - _v[0].head(2);
    // Eigen::Vector2f BC = _v[2].head(2) - _v[1].head(2);
    // Eigen::Vector2f CA = _v[0].head(2) - _v[2].head(2);

    // Eigen::Vector2f AP = p.head(2) - _v[0].head(2);
    // Eigen::Vector2f BP = p.head(2) - _v[1].head(2);
    // Eigen::Vector2f CP = p.head(2) - _v[2].head(2);
    
    // // 判断每个z坐标是否统一
	// return AB[0] * AP[1] - AB[1] * AP[0] > 0 
	// 	&& BC[0] * BP[1] - BC[1] * BP[0] > 0
	// 	&& CA[0] * CP[1] - CA[1] * CP[0] > 0;
 
    Vector3f p;
    p<<x,y,1.0f;
    Eigen::Vector3f AB = _v[1] - _v[0];
    Eigen::Vector3f BC = _v[2] - _v[1];
    Eigen::Vector3f CA = _v[0] - _v[2];

    Eigen::Vector3f AP = p - _v[0];
    Eigen::Vector3f BP = p - _v[1];
    Eigen::Vector3f CP = p - _v[2];

    
    Eigen::Vector3f c1 = AB.cross(AP);
    Eigen::Vector3f c2 = BC.cross(BP);
    Eigen::Vector3f c3 = CA.cross(CP);

    // std::cout<<"x,y:"<< x<<","<<y<<std::endl;
    // std::cout<<" AP:"<< AP.x()<<","<< AP.y()<<","<< AP.z()<<std::endl;
    // std::cout<<" c1:"<< c1.x()<<","<< c1.y()<<","<< c1.z()<<std::endl;
    // std::cout<<"c z:"<< c1.z()<<","<< c2.z()<<","<< c3.z()<<std::endl;
    
    if(c1.z()>0&&c2.z()>0&&c3.z()>0)
    {
        return true;
    }

    if(c1.z()<0&&c2.z()<0&&c3.z()<0)
    {
        return true;
    }

    return false;
}

static std::tuple<float, float, float> computeBarycentric2D(float x, float y, const Vector3f* v)
{
    float c1 = (x*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*y + v[1].x()*v[2].y() - v[2].x()*v[1].y()) / (v[0].x()*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*v[0].y() + v[1].x()*v[2].y() - v[2].x()*v[1].y());
    float c2 = (x*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*y + v[2].x()*v[0].y() - v[0].x()*v[2].y()) / (v[1].x()*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*v[1].y() + v[2].x()*v[0].y() - v[0].x()*v[2].y());
    float c3 = (x*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*y + v[0].x()*v[1].y() - v[1].x()*v[0].y()) / (v[2].x()*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*v[2].y() + v[0].x()*v[1].y() - v[1].x()*v[0].y());
    return {c1,c2,c3};
}

// Bresenham's line drawing algorithm
// Code taken from a stack overflow answer: https://stackoverflow.com/a/16405254
void rst::rasterizer::draw_line(Eigen::Vector3f begin, Eigen::Vector3f end)
{
    auto x1 = begin.x();
    auto y1 = begin.y();
    auto x2 = end.x();
    auto y2 = end.y();

    Eigen::Vector3f line_color = {255, 255, 255};

    int x,y,dx,dy,dx1,dy1,px,py,xe,ye,i;

    dx=x2-x1;
    dy=y2-y1;
    dx1=fabs(dx);
    dy1=fabs(dy);
    px=2*dy1-dx1;
    py=2*dx1-dy1;

    if(dy1<=dx1)
    {
        if(dx>=0)
        {
            x=x1;
            y=y1;
            xe=x2;
        }
        else
        {
            x=x2;
            y=y2;
            xe=x1;
        }
        Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
        set_pixel(point,line_color);
        for(i=0;x<xe;i++)
        {
            x=x+1;
            if(px<0)
            {
                px=px+2*dy1;
            }
            else
            {
                if((dx<0 && dy<0) || (dx>0 && dy>0))
                {
                    y=y+1;
                }
                else
                {
                    y=y-1;
                }
                px=px+2*(dy1-dx1);
            }
//            delay(0);
            Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
            set_pixel(point,line_color);
        }
    }
    else
    {
        if(dy>=0)
        {
            x=x1;
            y=y1;
            ye=y2;
        }
        else
        {
            x=x2;
            y=y2;
            ye=y1;
        }
        Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
        set_pixel(point,line_color);
        for(i=0;y<ye;i++)
        {
            y=y+1;
            if(py<=0)
            {
                py=py+2*dx1;
            }
            else
            {
                if((dx<0 && dy<0) || (dx>0 && dy>0))
                {
                    x=x+1;
                }
                else
                {
                    x=x-1;
                }
                py=py+2*(dx1-dy1);
            }
//            delay(0);
            Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
            set_pixel(point,line_color);
        }
    }
}


void rst::rasterizer::draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, col_buf_id col_buffer, Primitive type)
{
    auto& buf = pos_buf[pos_buffer.pos_id];
    auto& ind = ind_buf[ind_buffer.ind_id];
    auto& col = col_buf[col_buffer.col_id];

    float f1 = (50 - 0.1) / 2.0;
    float f2 = (50 + 0.1) / 2.0;

    Eigen::Matrix4f mvp = projection * view * model;
    for (auto& i : ind)
    {
        Triangle t;
        Eigen::Vector4f v[] = {
                mvp * to_vec4(buf[i[0]], 1.0f),
                mvp * to_vec4(buf[i[1]], 1.0f),
                mvp * to_vec4(buf[i[2]], 1.0f)
        };
        //Homogeneous division
        for (auto& vec : v) {
            vec /= vec.w();
        }
        //Viewport transformation
        for (auto & vert : v)
        {
            vert.x() = 0.5*width*(vert.x()+1.0);
            vert.y() = 0.5*height*(vert.y()+1.0);
            vert.z() = vert.z() * f1 + f2;
        }

        for (int i = 0; i < 3; ++i)
        {
            t.setVertex(i, v[i].head<3>());
            t.setVertex(i, v[i].head<3>());
            t.setVertex(i, v[i].head<3>());
        }

        auto col_x = col[i[0]];
        auto col_y = col[i[1]];
        auto col_z = col[i[2]];

        t.setColor(0, col_x[0], col_x[1], col_x[2]);
        t.setColor(1, col_y[0], col_y[1], col_y[2]);
        t.setColor(2, col_z[0], col_z[1], col_z[2]);

        rasterize_triangle(t);
    }
}

bool rst::rasterizer::get_mssa(int mssa,float x,float y,Vector3f _v, float & result)
{
    int ret = false;

    return ret;
}

//Screen space rasterization
void rst::rasterizer::rasterize_triangle(const Triangle& t) {
    auto v = t.toVector4();
    
    // draw_line(t.c(), t.a());
    // draw_line(t.c(), t.b());
    // draw_line(t.b(), t.a());

    // TODO : Find out the bounding box of current triangle.
    // iterate through the pixel and find if the current pixel is inside the triangle

    // If so, use the following code to get the interpolated z value.
    // auto[alpha, beta, gamma] = computeBarycentric2D(1, 2, t.v);
    //float w_reciprocal = 1.0/(alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
    //float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
    //z_interpolated *= w_reciprocal;

    // TODO : set the current pixel (use the set_pixel function) to the color of the triangle (use getColor function) if it should be painted.
    
    float max_x = std::max(v[0].x(),std::max(v[1].x(),v[2].x()));
    float max_y = std::max(v[0].y(),std::max(v[1].y(),v[2].y()));
    
    float min_x = std::min(v[0].x(),std::min(v[1].x(),v[2].x()));
    float min_y = std::min(v[0].y(),std::min(v[1].y(),v[2].y()));

    max_x = std::ceil(max_x);
    max_y = std::ceil(max_y);

    min_x = std::floor(min_x);
    min_y = std::floor(min_y);

    // std::cout<<"  x:"<<min_x<<","<<max_x<<std::endl;
    // std::cout<<"  y:"<<min_y<<","<<max_y<<std::endl;
	
    bool MSAA = true;
	//MSAA 4X
	if (MSAA) {
		// 格子里的细分四个小点坐标
		std::vector<Eigen::Vector2f> pos
		{
			{0.25,0.25},
			{0.75,0.25},
			{0.25,0.75},
			{0.75,0.75},
		};
		for (int i = min_x; i <= max_x; i++) {
			for (int j = min_y; j <= max_y; j++) {
				// 记录最小深度
				float minDepth = 1000;
				// 四个小点中落入三角形中的点的个数
				int count = 0;
				// 对四个小点坐标进行判断 
				for (int n = 0; n < 4; n++) {
					// 小点是否在三角形内
					if (insideTriangle((float)i + pos[n][0], (float)j + pos[n][1], t.v)) {
						// 如果在，对深度z进行插值
						auto tup = computeBarycentric2D((float)i + pos[n][0], (float)j + pos[n][1], t.v);
						float alpha;
						float beta;
						float gamma;
						std::tie(alpha, beta, gamma) = tup;
						float w_reciprocal = 1.0 / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
						float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
						z_interpolated *= w_reciprocal;
						minDepth = std::min(minDepth, z_interpolated);
						count++;
					}
				}
				if (count != 0) {
					if (depth_buf[get_index(i, j)] > minDepth) {
						Vector3f color = t.getColor() * count / 4.0;
						Vector3f point(3);
						point << (float)i, (float)j, minDepth;
						// 替换深度
						depth_buf[get_index(i, j)] = minDepth;
						// 修改颜色
						set_pixel(point, color);
					}
				}
			}
		}
	}
    else 
    {
        for(int i = min_x;i<=max_x;i++)
        {
            for(int j = min_y;j<=max_y;j++)
            {
                float minDepth = 1000.0;
                if(insideTriangle(i,j,t.v))
                {   
                    auto tup = computeBarycentric2D(i, j, t.v); 
                    float alpha;
                    float beta;
                    float gamma;   
                    std::tie(alpha,beta,gamma) = tup;

                    // If so, use the following code to get the interpolated z value.
                    // auto[alpha, beta, gamma] = computeBarycentric2D(1, 2, t.v);
                    float w_reciprocal = 1.0/(alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
                    float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
                    z_interpolated *= w_reciprocal;
                    if(z_interpolated<depth_buf[get_index(i,j)])
                    {
                        depth_buf[get_index(i,j)] = z_interpolated;
                        set_pixel(Vector3f(i,j,1.0f),t.getColor());
                    }            
                }
            }  
        } 
    }
}

void rst::rasterizer::set_model(const Eigen::Matrix4f& m)
{
    model = m;
}

void rst::rasterizer::set_view(const Eigen::Matrix4f& v)
{
    view = v;
}

void rst::rasterizer::set_projection(const Eigen::Matrix4f& p)
{
    projection = p;
}

void rst::rasterizer::clear(rst::Buffers buff)
{
    if ((buff & rst::Buffers::Color) == rst::Buffers::Color)
    {
        std::fill(frame_buf.begin(), frame_buf.end(), Eigen::Vector3f{0, 0, 0});
    }
    if ((buff & rst::Buffers::Depth) == rst::Buffers::Depth)
    {
        std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::infinity());
    }
}

rst::rasterizer::rasterizer(int w, int h) : width(w), height(h)
{
    frame_buf.resize(w * h);
    depth_buf.resize(w * h);
}

int rst::rasterizer::get_index(int x, int y)
{
    return (height-1-y)*width + x;
}

void rst::rasterizer::set_pixel(const Eigen::Vector3f& point, const Eigen::Vector3f& color)
{
    //old index: auto ind = point.y() + point.x() * width;
    auto ind = (height-1-point.y())*width + point.x();
    frame_buf[ind] = color;

}

// clang-format on