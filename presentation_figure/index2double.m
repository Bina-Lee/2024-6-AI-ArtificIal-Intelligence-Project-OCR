% 초성, 중성, 종성의 인덱스 범위 설정
first_idx = 0:18;  % 초성 범위 (0 ~ 18)
middle_idx = 0:20; % 중성 범위 (0 ~ 20)
last_idx = 0:27;   % 종성 범위 (0 ~ 27)

% 매핑된 값 계산
first_var = (first_idx + 1) / 21.0;  % 초성 매핑: (idx + 1) / 21
middle_var = (middle_idx + 1) / 23.0; % 중성 매핑: (idx + 1) / 23
last_var = (last_idx + 1) / 30.0;    % 종성 매핑: (idx + 1) / 30

% 그래프 그리기
figure;
hold on;

plot(first_idx, first_var, '-o', 'DisplayName', '초성 매핑: (idx + 1) / 21');
plot(middle_idx, middle_var, '-x', 'DisplayName', '중성 매핑: (idx + 1) / 23');
plot(last_idx, last_var, '-s', 'DisplayName', '종성 매핑: (idx + 1) / 30');

xlabel('Index');
ylabel('Mapped Value');
title('초성, 중성, 종성 매핑 그래프');
legend('Location', 'best');
grid on;

hold off;

% % 초성, 중성, 종성의 정수 범위
% first_indices = 0:18;  % 초성: 0 ~ 18
% middle_indices = 0:20; % 중성: 0 ~ 20
% last_indices = 0:27;   % 종성: 0 ~ 27
% 
% % 1. `convert_IDX2VAR`로 유리수 계산
% first_values = (first_indices + 1) / 21.0;   % 초성
% middle_values = (middle_indices + 1) / 23.0; % 중성
% last_values = (last_indices + 1) / 30.0;     % 종성
% 
% % 2. 반올림하여 스텝 함수용 X축과 Y축 계산
% [first_x, first_y, first_marker_x, first_marker_y] = generate_step_function_with_central_marker(first_values, round(first_values * 21.0) - 1);
% [middle_x, middle_y, middle_marker_x, middle_marker_y] = generate_step_function_with_central_marker(middle_values, round(middle_values * 23.0) - 1);
% [last_x, last_y, last_marker_x, last_marker_y] = generate_step_function_with_central_marker(last_values, round(last_values * 30.0) - 1);
% 
% % 3. 그래프 그리기
% figure;
% plot(first_x, first_y, '-', 'DisplayName', '초성 (First)'); hold on;
% plot(first_marker_x, first_marker_y, 'o', 'DisplayName', '초성 점'); % 중앙에만 점
% plot(middle_x, middle_y, '-', 'DisplayName', '중성 (Middle)');
% plot(middle_marker_x, middle_marker_y, 'o', 'DisplayName', '중성 점');
% plot(last_x, last_y, '-', 'DisplayName', '종성 (Last)');
% plot(last_marker_x, last_marker_y, 'o', 'DisplayName', '종성 점');
% 
% % 4. 그래프 설정
% xlabel('Normalized Value');
% ylabel('Index');
% title('Step Function with Central Markers');
% legend('show');
% grid on;
% 
% % ----------------------------------------------
% % 함수: 스텝 함수 생성 (중앙에 점 추가)
% function [step_x, step_y, marker_x, marker_y] = generate_step_function_with_central_marker(values, rounded_indices)
%     % values: 유리수 값
%     % rounded_indices: 반올림된 정수 값
% 
%     step_x = [];
%     step_y = [];
%     marker_x = [];
%     marker_y = [];
% 
%     for i = 1:length(values)-1
%         % 스텝의 시작점과 끝점
%         step_x = [step_x, values(i), values(i+1)];
%         step_y = [step_y, rounded_indices(i), rounded_indices(i)];
% 
%         % 중앙에 점 찍기
%         marker_x = [marker_x, (values(i) + values(i+1)) / 2];
%         marker_y = [marker_y, rounded_indices(i)];
%     end
% 
%     % 마지막 값을 추가
%     step_x = [step_x, values(end)];
%     step_y = [step_y, rounded_indices(end)];
% end