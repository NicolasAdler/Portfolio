numRuns = 10;
all_rmse = zeros(numRuns,1);
all_mae  = zeros(numRuns,1);
all_r    = zeros(numRuns,1);
all_pval = zeros(numRuns,1);

for run = 1:numRuns
    % Load data
    realData = readtable('PositionalLevel_All_Band_STI_Data_MeasurmentPhase.txt', 'Delimiter', '\t');
    simData  = readtable('PositionalLevel_All_Band_STI_Data_SimPhase.txt', 'Delimiter', '\t');
    realData.Properties.VariableNames{1} = 'Room';
    simData.Properties.VariableNames{1}  = 'Room';

    predictorNames = {'T60_500','T60_1000','T60_2000'};


    realData = realData(~any(ismissing(realData(:, [{'STI'} predictorNames])), 2), :);
    simData  = simData(~any(ismissing(simData(:,  [{'STI'} predictorNames])), 2), :);

    realHalls = unique(realData.Room);
    simHalls = unique(simData.Room);
    rng('shuffle');
    testRealHalls = randsample(realHalls, 2);
    testSimHalls = randsample(simHalls, 2);

    isTestReal = ismember(realData.Room, testRealHalls);
    isTestSim  = ismember(simData.Room, testSimHalls);
    realTest = realData(isTestReal, :);
    realTrain = realData(~isTestReal, :);
    simTest = simData(isTestSim, :);
    simTrain = simData(~isTestSim, :);

    trainData = [realTrain; simTrain];
    testData  = [realTest; simTest];

    for k = 1:length(predictorNames)
        trainData.(predictorNames{k}) = log(trainData.(predictorNames{k}));
        testData.(predictorNames{k})  = log(testData.(predictorNames{k}));
    end

    X_train_table = array2table(trainData{:, predictorNames}, 'VariableNames', predictorNames);
    X_train_table.STI = trainData.STI;

    X_test_table = array2table(testData{:, predictorNames}, 'VariableNames', predictorNames);
    y_test = testData.STI;

    layers = [
        featureInputLayer(numel(predictorNames))
        fullyConnectedLayer(64)
        reluLayer
        dropoutLayer(0.2)
        fullyConnectedLayer(32)
        reluLayer
        dropoutLayer(0.2)
        fullyConnectedLayer(16)
        reluLayer
        fullyConnectedLayer(1)
        regressionLayer
        ];


    options = trainingOptions('adam', ...
    'MaxEpochs', 500, ...
    'MiniBatchSize', 8, ...
    'Shuffle', 'every-epoch', ...
    'Verbose', false, ...
    'Plots', 'none', ...
    'ValidationData', {table2array(X_test_table(:, predictorNames)), y_test}, ...
    'ValidationFrequency', 10, ...
    'ValidationPatience', 20);

    mlp = trainNetwork(X_train_table, 'STI', layers, options);

    y_pred = predict(mlp, X_test_table);

    rmse = sqrt(mean((y_pred - y_test).^2));
    mae  = mean(abs(y_pred - y_test));
    r = corr(y_test, y_pred);
    all_rmse(run) = rmse;
    all_mae(run)  = mae;
    all_r(run)    = r;

    fprintf('\n[Run %d] RMSE: %.4f, MAE: %.4f, r: %.4f\n', run, rmse, mae, r);
end

fprintf('\n--- AVERAGED OVER %d RUNS ---\n', numRuns);
fprintf('Avg r:    %.4f\n', mean(all_r));
fprintf('Avg RMSE: %.4f\n', mean(all_rmse));
fprintf('Avg MAE:  %.4f\n', mean(all_mae));
