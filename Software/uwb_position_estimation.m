%������ӽ���ƽ��λ�ù���
clear all;
syms a b x y;
p1 = [0;0];
p2 = [a;b];
p3 = [x;y];
P = [p1;p2;p3];
BETA = [p2;p3];
deltaBETA = [0;0;0;0];
fp = [norm(p1-p2);norm(p1-p3);norm(p2-p3)];
D = [normrnd(5,0.05);normrnd(5,0.05);normrnd(50^0.5,0.05)]; 
p20 = [5;0.01];%p2 ��ʼλ��
p30 = [0.01;5];%p3 ��ʼλ��
global INIT;
INIT = [p20;p30];
data = zeros(5000,4);

nlr_time=10;%����10��
x1=zeros(nlr_time,1);
x2=zeros(nlr_time,1);
y1=zeros(nlr_time,1);
y2=zeros(nlr_time,1);

% NLR λ�ù���
for i=1:1:nlr_time
    x1(i)=INIT(1);
    x2(i)=INIT(3);
    y1(i)=INIT(2);
    y2(i)=INIT(4);

    Jk = subs(jacobian(fp,BETA),[p2,p3],[INIT(1:2),INIT(3:4)]);
    deltaD = D - subs(fp,[p2,p3],[INIT(1:2),INIT(3:4)]);

    %syms w1 w2 w3;
    W = [1 0 0;0 1 0;0 0 1];

    %deltaBETA = double(pinv(double(Jk'*Jk))*Jk'*deltaD);
    deltaBETA = double(pinv(double(Jk))*0.6*deltaD);
    INIT = INIT + deltaBETA;
end

double(INIT)
pos2 = p20;
uwb_pos2 = [INIT(1);INIT(2)];
velocity2 = [0;0];
distance2 = [0;0]; %distance and last distance
uwb_Data = [0;0;0;0];
Gamma = 0.1;
time = 50;
sampling_period = 0.05;
rl_error = zeros(1,1);
count = 0;
analyze = [0,0.0,0];

% ���ڹ�ʶ��λ�ù��ƣ��ں��ٶȣ�
for step=0:sampling_period:time
    
    velocity2(1) = -sin(step)*50;
    velocity2(2) = cos(step)*50;

    pos2(1) = pos2(1) + velocity2(1)*sampling_period;
    pos2(2) = pos2(2) + velocity2(2)*sampling_period;
    
    distance2(2) = distance2(1);
    distance2(1) = sqrt(pos2(1)^2 + pos2(2)^2);
    uwb_Data = [velocity2(1) + normrnd(0,0.1);velocity2(2) + normrnd(0,0.1);distance2(1) + normrnd(0,0.05);distance2(2) + normrnd(0,0.05)];%x�ٶ� y�ٶ� ����uwb���� �ϴ�uwb���� ������
    uwb_pos2 = uwb_pos2 + sampling_period*[uwb_Data(1);uwb_Data(2)];
    rl_error = [rl_error,sqrt((uwb_pos2(1)-pos2(1))^2 + (uwb_pos2(2)-pos2(2))^2)];

    subplot(1,2,2)
    %axis([-7,7,-7,7])  %ȷ��x����y���ͼ��С
    plot(uwb_pos2(1),uwb_pos2(2),'*b',pos2(1),pos2(2),'.r');hold on;
    title('UAV1λ��(m)');hold on;
    xlabel('xλ�ã�m��');hold on;
    ylabel('yλ�ã�m��');hold on;
end
subplot(1,2,1)
    plot(rl_error);hold on;
    title('��Զ�λ�������(m)');hold on;
    ylabel('��m��');hold on;
    xlabel('��������');hold on;


   