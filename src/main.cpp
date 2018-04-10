#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}
double p[3]{0.380643, 0.0309032, 4.65811};
double dp[3]{1,1,1};
int counter = 0;
double tol = 0.2;
bool isCalculateErr = false;
bool isInit = false;
double best_err = 0.0;
double m_error = 0;
int para_index = 0;
//current parameter index
int current_index = 0;
//current twiddle step: 1; 2; 3
enum Step{STEP_1, STEP_2};
Step phase = STEP_1;

int main()
{
  uWS::Hub h;
  
  PID pid;
  // TODO: Initialize the pid variable.
//[2.9331227688652457, 10.326589894591526, 0.49316041639454505]
  // double m_Kp = -0.5;
  // double m_Ki = -0.004;
  // double m_Kd = -0.5;

  pid.Init(p[0], p[1], p[2]);
  

  

  h.onMessage([&pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value;
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1]. 
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */
          // counter++;
          // if(!isInit)
          // {
          //   if(counter > 100 && counter < 200)
          //   {
          //     // std::string msg = "42[\"reset\",{}]";
          //     // std::cout << msg << std::endl;
          //     // ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
          //     m_error += cte*cte;
          //   }
          //   if(counter >= 200){
          //     //get init best_err
          //     best_err = m_error;
          //     //initialization completed
          //     isInit = true;
          //     //reset counter for twiddle
          //     counter = 0;
          //     std::cout<<"initialization completed with error: "<<best_err<<"\n";
          //     //reset simulater
          //     std::string msg = "42[\"reset\",{}]";
          //     std::cout << msg << "\n";
          //     std::cout <<"Update PID: get error < "<<best_err<<"\nP: "<<p[0]<<"\tI: "<<p[1]<<"\tD: "<<p[2]<<std::endl;
          //     ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
          //   }
          // }else
          // {
          //   // std::cout<<"start twiddling \n";
          //   //if cannot improve performance restart 
          //   if((dp[0]+dp[1]+dp[2] < tol))
          //   { //abs(cte) > 6.0 || 
          //     std::string msg = "42[\"restart\",{}]";
          //     std::cout << msg << "\n";
          //     std::cout <<"Get error < "<<best_err<<"\nP: "<<p[0]<<"\tI: "<<p[1]<<"\tD: "<<p[2]<<std::endl;
          //     ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
          //     counter = 0;
          //   }else{

          //     switch(phase)
          //     {
          //       case STEP_1: if(!isCalculateErr)
          //                     {//try twiddle para_index-th parameter
          //                       std::cout<<"Step 1!\n";
          //                       current_index = para_index ;
          //                       //update parameter index for next update
          //                       para_index = (para_index+1)%3;
          //                       //twiddle coefficient
          //                       p[current_index] += dp[current_index];
          //                       //update coefficients of pid controller
          //                       // pid.Kp = p[0];
          //                       // pid.Ki = p[1];
          //                       // pid.Kd = p[2];
          //                       pid.Init(p[0], p[1], p[2]);
          //                       isCalculateErr = true;
          //                       m_error = 0;
          //                     }else{
          //                       // std::cout<<"Step 1: calculating errors!\n";
          //                       //calculating err from 100 iteration
          //                       if( counter > 100 && counter < 200)
          //                       {
          //                         m_error += cte*cte;
          //                       }
          //                       if(counter >= 200)
          //                       {
          //                         counter = 0;
          //                         isCalculateErr = false;
          //                         //if got better error update best_err
          //                         if( m_error < best_err )
          //                         {
          //                           best_err = m_error;
          //                           dp[current_index]*=1.1;
          //                           current_index = para_index;
                                
          //                         }else{
          //                           phase = STEP_2;
          //                         }

          //                         //reset simulater
          //                         std::string msg = "42[\"reset\",{}]";
          //                         // std::cout << msg << "\n";
          //                         std::cout <<"Step 1: update PID: get error < "<<best_err<<"\nP: "<<p[0]<<"\tI: "<<p[1]<<"\tD: "<<p[2]<<std::endl;
          //                         ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);

          //                       }
          //                     }
                              
          //                     break;

          //       case STEP_2: if(!isCalculateErr)
          //                     {
          //                       std::cout<<"Step 2!\n";
          //                       p[current_index] -= 2*dp[current_index];
          //                       //update coefficients of pid controller
          //                       // pid.Kp = p[0];
          //                       // pid.Ki = p[1];
          //                       // pid.Kd = p[2];
          //                       pid.Init(p[0], p[1], p[2]);
          //                       isCalculateErr = true;
          //                       m_error = 0;
          //                     }else{
          //                       //calculating err from 100 iteration
          //                       if( counter > 100 && counter < 200)
          //                       {
          //                         m_error += cte*cte;
          //                       }
          //                       if(counter >= 200)
          //                       {
          //                         counter = 0;
          //                         isCalculateErr = false;
          //                         //if got better error update best_err
          //                         if( m_error < best_err )
          //                         {
          //                           best_err = m_error;
          //                           dp[current_index]*=1.1;
                                    
          //                         }else{
          //                           p[current_index] += dp[current_index];
          //                           dp[current_index] *= 0.9;

          //                         }
          //                         //move to next parameter
          //                         current_index = para_index;
          //                         //restart from step 1
          //                         phase = STEP_1;
          //                         //reset simulater
          //                         std::string msg = "42[\"reset\",{}]";
          //                         // std::cout << msg << "\n";
          //                         std::cout <<"Step 2: update PID: get error < "<<best_err<<"\nP: "<<p[0]<<"\tI: "<<p[1]<<"\tD: "<<p[2]<<std::endl;
          //                         ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);

          //                       }
          //                     }
                              
          //                     break;
          //       default: std:: cout<<"error in twiddle steps!"<<std::endl;
          //     }

          //       //debug
          //       switch(current_index)
          //       {
          //         case 0: 
          //                 std::cout << "Twiddle PID --> P: "<<p[0]<<"\n";
          //                 break;
          //         case 1: 
          //                 std::cout << "Twiddle PID --> I: "<<p[1]<<"\n";
          //                 break;
          //         case 2: 
          //                 std::cout << "Twiddle PID --> D: "<<p[2]<<"\n";
          //                 break;
          //         default: 
          //                 std::cout << "Twiddle PID error index -->: "<<current_index<<"\n";
          //                 break;
          //       }           

          //   }
          // }
          //calculating steer
          pid.UpdateError(cte);
          steer_value = -pid.TotalError();
          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = 0.3;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          // std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
