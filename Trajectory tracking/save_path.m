function [ ] = save_path( tracks,save_file,min_length,casename,caseFile,save_paths,save_stability,trackNum)
%SAVE_PATH 此处显示有关此函数的摘要
%   此处显示详细说明
addpath(genpath('D:/c++/Stitching-1.0.0'));


point_x=tracks.points(:,:,1);
point_y=tracks.points(:,:,2);
num_of_path=tracks.nTrack;
frame_num=tracks.nFrame;
path=[];
%%
ratio=0;
ratio_num=0;
for i=1:num_of_path
    start=0;
    
    for t=1:frame_num
        
        if point_x(i,t)~=0
          start=t;
          break;
        end
        
    end
    
    single_path_x=point_x(i,:);
    single_path_x=single_path_x(single_path_x~=0);
    
    single_path_y=point_y(i,:);
    single_path_y=single_path_y(single_path_y~=0);
    
    end_frame=start+max(size(single_path_x))-1;
    
    single_path=[[start,end_frame];[single_path_x',single_path_y']];
    
    length_of_single_path = size(single_path); 
    
    if length_of_single_path(1,1)>=min_length
       path=[path;single_path];
       
       %添加测试stability
       for m=1:length_of_single_path(1,1)-40
          P_start=single_path(m,:);
          P_end=single_path(m+39,:);
          PP=single_path(m:m+39,:);
          len1=sqrt((P_start(1)-P_end(1))^2+(P_start(2)-P_end(2))^2);
          len2=0;
          for n=1:39
              len2=len2+sqrt((PP(n,1)-PP(n+1,1))^2+(PP(n,2)-PP(n+1,2))^2);
          end
          ratio=ratio+len1/len2;
          ratio_num=ratio_num+1;
       end
       
    end
    
     clear single_path_x;
     clear single_path_y;
    
end
ratio=ratio/ratio_num

%%
% save_file=strcat(save_file,'paths/',casename,'_path','.txt');
if(save_paths==1)
    mkdir(strcat(save_file,'paths',num2str(trackNum)));
    save(strcat(save_file,'paths',num2str(trackNum),'/',casename,'_path','.txt'),'path','-ascii');
end
% save D:/data/video33_path.txt path -ascii;
display('over');
if(save_stability==1)
%     cell_str = strsplit(caseFile,'/');  %分成三段： 'P1'    'front'    'img.jpg'
%     smap_name = strcat(cell_str{1,4},'_',cell_str{1,5});
    fid=fopen(strcat(save_file,'stability.txt'),'a');
    fprintf(fid,'%s %10.4f\n',casename,ratio);
    fclose(fid);
end

end

