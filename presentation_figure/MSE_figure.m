clear;
% CSV 파일 경로 설정
file_list = {
    'training_log_10_4.csv',
    'training_log_10_8.csv',
    'training_log_10_16.csv',
    'training_log_10_32.csv',
    'training_log_10_64.csv',
    'training_log_10_128.csv',
    'training_log_10_256.csv',
    'training_log_10_toLow.csv'
};

legend_names = { % 사용자 정의 범례 이름
    'Node: 4',
    'Node: 8',
    'Node: 16',
    'Node: 32',
    'Node: 64',
    'Node: 128',
    'Node: 256',
    'Low Config'
};
% file_list={
%     'training_log_He_ReLU.csv',
%     'training_log_He_Sigmoid.csv',
%     'training_log_He_Tanh.csv',
%     'training_log_Xavier_ReLU.csv',
%     'training_log_Xavier_Sigmoid.csv',
%     'training_log_Xavier_Tanh.csv'
% };
% 
% legend_names={
%     'He, ReLU',
%     'He, Sigmoid',
%     'He, Tanh',
%     'Xavier, ReLU',
%     'Xavier, Sigmoid',
%     'Xavier, Tanh'
% };

% 그래프 초기화
figure;
hold on;
xlabel('Epoch');
ylabel('MSE');
title('MSE Across Different Training Logs');
grid on;

% 파일별 데이터 처리 및 플로팅
for i = 1:length(file_list)
    filename = file_list{i}; % 현재 파일 이름
    data = readmatrix(filename); % CSV 파일 읽기
    
    % 데이터 크기 확인
    if size(data, 2) < 5
        error('파일 %s 에 다섯 번째 열이 존재하지 않습니다.', filename);
    end
    
    % 다섯 번째 열 데이터 가져오기
    fifth_column = data(1:10:end, 5); % 다섯 번째 열의 모든 행 데이터
    
    % 데이터 플로팅
    plot(1:10:5001,fifth_column, '-', 'DisplayName', legend_names{i}, 'LineWidth',2); % 범례에 파일명 추가
end

% 범례 추가
legend('show', 'Location', 'best');

axis([0 5000 0 0.11]);

grid minor