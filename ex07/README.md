\# ex07 - Web网页端无极调光器



\## 实验目的



使用 ESP32 Web Server 和 PWM 实现网页端无极调光功能。



\## 实验现象



手机或电脑浏览器打开 ESP32 网页后，页面中显示一个滑动条。拖动滑动条时，开发板上的 LED 亮度会随着滑动条位置实时、平滑地改变。



\## 实验原理



网页端使用 input range 滑动条获取 0 到 255 的亮度值，并通过 JavaScript 的 fetch() 请求发送给 ESP32。ESP32 解析 URL 参数 value，并使用 analogWrite() 改变 LED 的 PWM 占空比，从而调节 LED 亮度。

