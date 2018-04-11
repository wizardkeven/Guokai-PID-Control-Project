# Reflection

## 1. Effect of the PID component

PID controller is composed of three components: 
- P -> Proportional component
- I -> Integral component 
- D -> differential component. 

It uses a linear formula for calculating the output steering angle given **CTE** ( Cross Track Error). It's hard to get a clear understanding of effects which these components have on output steering angle through course video and on-class practices. They were no more than given conceptions thus I got no clue about what role they are playing in PID controller.

This project itself is not as complicated as the previous ones. It took me several hours to figure out the code structure and implement. Here the teaching video attached on the project page really helps. After a roughly browsing, it became even simpler and intuitive. However the really challenge came after my few trials on manual tuning coefficients Kp, Ki and Kd of the PID controller. I tried to set them as given by course video and teaching video as 0.5, 0.049 and 0.5 and tuned them around these values thinking they might be the "given optimal values" by "kind udacity team" to save time for us. But it failed. The car was driven no more than 10 m directly out of road and stuck into curb.

At this time, I remembered some details on course about differential part of PID. I reviewed that part and got to realize the proportional control **P** will response with direct feedback control according to current **CTE**. This component will take the error and adjust steering angle proportionally(the intensity is given by the component coefficient **Kp** in code) to reduce CTE and drive car back to aimed location. According to my test it proved this. So it needs to add the time differential component to  I tried reset them as 0.0 and start from Kp = 0.5 leaving other two as 0.0. In this case, the car was driven dramatically with over-adjustment according to CTE. Then I tried to add Kd to make it smoother. After some trials the car can start smooth and run smoother for beginning but will swing worse and worse with time passing by. Well it seems 
