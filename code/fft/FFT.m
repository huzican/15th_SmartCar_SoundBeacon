[wave,fs]=audioread('C:\Users\ASUS\Desktop\fft\chirp2.wav');
% sound(wave,fs);
wave(length(wave)+1:length(wave)+2000)=zeros(2000,1);
wave(length(wave));
subplot(2,2,1);
plot(wave);
subplot(2,2,2);
plot(fs);
a=xcorr(wave,wave);
subplot(2,2,3)
plot(a);

% Fs = 100;               % 采样频率100hz           
% T = 1/Fs;               % 采样周期       
% L = 2000;               % 采样数据点个数
% x = (0:L-1)*T;          % 采样时间表
% 
% y1 = 1024 * cos(3 * pi * x) + 512 * cos(7 * pi * x + pi / 2) + 2047 % 建立函数
% 
% y2 = fft(y1);           % 做FFT运算
% 
% p = abs(y2*2)/(L)       % 计算幅值，幅值等于复数的模/(L/2)
% p = p(1:L/2)            % 取前半部分
% n = 0:L-1               % 创建横坐标
% f = Fs*n/L              % 计算横坐标的频率
% f = f(1:L/2)            % 取横坐标的前半部分  通常我们绘制幅频图只取前面一半进行绘制
% %为什么取一半呢，这里简单的说明一下，我们可以观察计算出来的横坐标频率，会看到频率是0-100的样子，
% %我们学过奈奎斯特定理都知道，100hz的采样频率只能完整采样50hz频率的波形。
% 
% plot(f,p)               % 绘制幅频图像
% title('Single-Sided Amplitude Spectrum of y1(f)')
% xlabel('频率(Hz)')        
% ylabel('幅值')
