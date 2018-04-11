# Reflection

## 1. Effect of the PID component

PID controller is composed of three components: 
- P -> Proportional component
- I -> Integral component 
- D -> differential component. 

It uses a linear formula for calculating the output steering angle given **CTE** ( Cross Track Error). It's hard to get a clear understanding of effects which these components have on output steering angle through course video and on-class practices. They were no more than given conceptions thus I got no clue about what role they are playing in PID controller.

This project itself is not as complicated as the previous ones. It took me several hours to figure out the code structure and implement. Here the teaching video attached on the project page really helps. After a roughly browsing, it became even simpler and intuitive. However the really challenge came after my few trials on manual tuning coefficients Kp, Ki and Kd of the PID controller. I tried to set them as given by course video and teaching video as 0.5, 0.049 and 0.5 and tuned them around these values thinking they might be the "given optimal values" by "kind udacity team" to save time for us. But it failed. The car was driven no more than 10 m and ended up with deviating of road and/or stuck in curb.

Obviously manual tuning doesn't seem to be an effective approach. It may work through long time experiments with appropriate approaching but it may also fail due to wrong initial value and bad tuning strategy. So why not try the algorithm **twiddle**. In fact, I decided to use manual tuning after browsing the project code because I found it not as trivial as the implementation in the course. In project code, the car is driving and sensing in real-time and we could not run the car for arbitrary time within a loop. All the steps have to be implemented in real time driving environment. So I turned to forum for some hints. As what I expected, I am not alone. Checked several posts, I got one important tip, which by sending an empty package with command "restart" we can force the simulator to get restarted without manually interference. But what I want is not restart the simulator but reset the car to starting point. I guess "reset" may work if "restart" works. I tried and it worked as expected. Well I got the most important tool to approach the "**twiddle**" algorithm. Then it took me another 4 hours to implement it. 

I ran the code with starting PID value as 0.0, step values as 1.0 and threshold sum of tuning steps as 0.2(as showed in course video) and finally the algorithm converged after more than 1 h. Now I really thank to my staring at the screen during the training process because this process showed me how the change of each component affect steering angle.   

Now I know better how each components of PID works. The proportional control **P** will response with direct feedback control according to current **CTE**. This component will take the error and adjust steering angle proportionally(the intensity varies according to the component coefficient **Kp** in code) to reduce CTE and drive car back to  track. 

Next, in order to smooth the control process it is necessary to add temporal differential component **D**.  After some trials the car can run more smoothly but will swing worse and worse with time passing by due to system noise. 

Finally, to mediate the deviation caused from system noise or control noise, the component **I** for integral component will play the role. By adding the accumulated error correction i.e I, the controller can eventually get rid of system noise and run through the whole lap with no severe drift.
