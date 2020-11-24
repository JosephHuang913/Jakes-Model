close;
close;
close;
clear;
amp = fopen('mag_pdf.log', 'r');
ph = fopen('ph_pdf.log', 'r');
magnitude = fopen('amplitude.log', 'r');
phase = fopen('phase.log', 'r');
amp_pdf = fscanf(amp, '%f');
ph_pdf = fscanf(ph, '%f');
mag_t = fscanf(magnitude, '%f');
phase_t = fscanf(phase, '%f');
covariance = fopen('autocovariance.log', 'r');
autocovariance = fscanf(covariance, '%f');

figure(1);
subplot(2,1,1);
hold on
i=1:1:400;
plot(i/100, amp_pdf, 'b');

sigma=sqrt(0.5);
r=0:0.01:4;
p=(r/sigma.^2).*exp(-r.^2/(2*sigma.^2));
r=1:1:400;
plot((r)/100, p(r), 'r');

title('PDF of Rayleigh Fading Signal');
xlabel('Amplitude (volt)');
ylabel('Probability Deisity');

s=0;
for i=1:1:400
 
    
    
    s=s+amp_pdf(i);
end
s*0.01
%s=0;
%for r=0:0.01:6
%   s=s+(r/sigma.^2).*exp(-r.^2/(2*sigma.^2));
%end
%s*0.01

subplot(2,1,2);
hold on;
i=1:1:800;
plot((i-400)/100, ph_pdf, 'b');
plot((i-400)/100, 1/(2*pi), 'r');

xlabel('Phase (rad)');
ylabel('Probability Deisity');
%print -djpeg100 Rayleigh_pdf.jpg

figure(2);
subplot(3,1,1);
hold on
i=1:1:2500;
plot(i/25000, 20*log10(mag_t(i)), 'b');
%axis([0 0.26 -30 10]);
%title('Amplitude of Rayleigh Fading Signal');
xlabel('Time (s)');
ylabel('Amplitude (dB volt)');

subplot(3,1,2);
hold on;
i=1:1:2500;
plot(i/25000, phase_t(i), 'b');
%axis([0 0.26 -4 4]);
%title('Phase of Rayleigh Fading Signal');
xlabel('Time (s)');
ylabel('Phase (rad)');

subplot(3,1,3);
hold on;
for i=2:1:2500;
    rand_ph(i)=phase_t(i)-phase_t(i-1);
    if(rand_ph(i) > pi)
        rand_ph(i)=rand_ph(i)-2*pi;
    elseif(rand_ph(i) < -pi)
        rand_ph(i)=rand_ph(i)+2*pi;
    end
end

i=2:1:2500;
%plot(i/1e4, 10000*rand_ph(i)/(2*pi), 'b');
plot(i/25000, rand_ph(i), 'b');
%axis([0 0.26 -0.5 1]);
%title('Random FM of Rayleigh Fading Signal');
xlabel('Time (s)');
ylabel('Random FM (rad)');
%print -djpeg100 Rayleigh_fading_pattern.jpg

%figure(3);
%i=1:1:289;
%plot(i/192, autocovariance);
%axis([0 1.5 0 0.25]);
%title('Autocovariance of Amplitude of the Rayleigh Fading Signal');
%xlabel('f_m*t Wavelength (lamda)');
%ylabel('Normalized Autocovariance');

%figure(4);
%plot(10*log10(psd));