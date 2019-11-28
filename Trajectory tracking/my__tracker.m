% casename={'2','4','video11','video14','video22','video26','video27','video28','video29','video31','video33','video38',...
%     'video39','video40','video41','video42','video43','video45','video46','video47','video48','video53',...
%     'video54','video55','video56','video70','video71','video72','video73','video78','video80','video81'};

% casename={'2me','2stb','4me','4stb','7me','7stb','9me','9stb','11me','11stb','13me','13stb','14me','14stb','15me','15stb','16me','16stb','19me','19stb','20me','20stb','21me','21stb','22me','22stb'};
% casename={'1','2','3','4','5','6','18','20','22'};
addpath(genpath('Stitching-1.0.0'));
save_paths=1;
save_stability=1;
save_images=0;
is_test=0;
method='PROPOSED';

classname={'Regular','Parallax','Crowd','Bundled/Crowd','para_1_20_10_40_20_error_2147483647/Regular','para_1_20_10_40_40_iter_1/Regular','Crowd','Parallax'};
for n=2:3
    caseFile=strcat('E:/data/',method,'/unstable/',classname{1,n},'/videos/');
%     caseFile=strcat('E:/data/',method,'/results/',classname{1,n},'/');
    num=length(dir(caseFile))-2;
    doc=dir(caseFile);
    casename=cell(1,num);
    for i=1:length(casename)
        casename{1,i}=doc(i+2).name;%num2str(i-1);
    end
    dbstop error;

    
    %%
    %参数和路径
    %视频所在路径
    computeVideoStabilityHandler = @computeVideoStabilityWindow;
    ;
    saveFile = strcat('E:/data/',method,'/unstable/',classname{1,n},'/');
%     saveFile = strcat('E:/data/',method,'/results/',classname{1,n},'/');
    if(is_test==1)    
        min_length = 50;
    else
        min_length = 10;
    end

    for i=13:length(casename)
%         if i==10
%             continue;
%         end
        %%
        videoFile=strcat(caseFile,'/',casename{1,i});
        videotraFile=strcat(caseFile,'/',casename{1,i},'.mat');

        %参数
        caseName=casename{1,i};

        nWinSize = 40;
        trackNum = 150;
        divNum = 10;
        %%
        %运行代码
        track=GetTracksFromVideo(videotraFile, videoFile, divNum, trackNum,casename{1,i},saveFile,save_images);
        save_path( track,saveFile,min_length,caseName,caseFile,save_paths,save_stability,trackNum);
    end
end

