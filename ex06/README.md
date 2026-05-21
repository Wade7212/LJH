\# ex06 - Dual PWM Police Light Effect



\## 实验目的



使用两个 LED 和 PWM 实现双通道警车灯效。



\## 实验现象



两个 LED 呈现平滑交替渐变闪烁效果。一个 LED 逐渐变亮时，另一个 LED 逐渐变暗。



\## 实验原理



程序使用两个 PWM 输出控制两个 LED 的亮度。LED A 的亮度为 brightness，LED B 的亮度为 255 - brightness，因此两个 LED 的亮度始终呈反相关系。

