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

% Fs = 100;               % ����Ƶ��100hz           
% T = 1/Fs;               % ��������       
% L = 2000;               % �������ݵ����
% x = (0:L-1)*T;          % ����ʱ���
% 
% y1 = 1024 * cos(3 * pi * x) + 512 * cos(7 * pi * x + pi / 2) + 2047 % ��������
% 
% y2 = fft(y1);           % ��FFT����
% 
% p = abs(y2*2)/(L)       % �����ֵ����ֵ���ڸ�����ģ/(L/2)
% p = p(1:L/2)            % ȡǰ�벿��
% n = 0:L-1               % ����������
% f = Fs*n/L              % ����������Ƶ��
% f = f(1:L/2)            % ȡ�������ǰ�벿��  ͨ�����ǻ��Ʒ�Ƶͼֻȡǰ��һ����л���
% %Ϊʲôȡһ���أ�����򵥵�˵��һ�£����ǿ��Թ۲��������ĺ�����Ƶ�ʣ��ῴ��Ƶ����0-100�����ӣ�
% %����ѧ���ο�˹�ض���֪����100hz�Ĳ���Ƶ��ֻ����������50hzƵ�ʵĲ��Ρ�
% 
% plot(f,p)               % ���Ʒ�Ƶͼ��
% title('Single-Sided Amplitude Spectrum of y1(f)')
% xlabel('Ƶ��(Hz)')        
% ylabel('��ֵ')
