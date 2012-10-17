#ifndef DIRECT_SHOW_DIRECTORY_H
#define DIRECT_SHOW_DIRECTORY_H

/****************************************************************************************************
* 各种媒体格式的目录结构
* HDD -- 按照 Content > File 的关系保存？
* 
* Blu-ray Disc(蓝光光盘存储视频时的格式，AVCHD)，分为BD_ROM、BD_R(Write-Once)、BD_RE(Rewritable)
*   Root
*    +-BDMV\ -- 蓝光电影文件夹
*      +-index.bdmv -- 用于保存BDMV目录的描述信息（或者是 INDEX.BDM ？），如 .mpls 文件的顺序和50i/60i的识别信息。
*      +-MovieObject.bdmv -- 存储一个或多个"MovieObject"的信息
*      +-AUXDATA\   -- 包含声音和字体文件
*        [+-sound.bdmv] -- 可选的包含“HDMV Interactive Graphic streams”的声音文件
*        [aaaaa.otf]    -- 存储"Text subtitle"的字体信息
*      +-BACKUP\    -- 包含index.bdmv、MovieObject.bdmv、PLAYLIST和CLIPINFO目录下的所有文件来进行备份
*      +-CLIPINFO\  
*        +-00000.clpi   -- 存储关联 Clip AV流文件的Clip Infomation(如ConnectionCondition--两
*            个Content连接的条件,PlayListMark等)的文件
*        +-xxxxx.clpi -- 不一定是顺序递增的
*      +-PLAYLIST\  -- 包含播放列表的数据库文件
*        +-00000.mpls   -- 存储Movie PlayLists的信息，每个播放列表对应一个文件
*        +-yyyyy.mpls
*      +-STREAM\    -- 包含AV流文件(Import时会生成 xxxxx.modd 文件，是什么？)
*        +-00000.m2ts   -- 包含 BDAV MPEG-2 transport stream，和 xxxxx.clpi 对应。是真正的数据文件
*        +-xxxxx.m2ts   -- (在NTFS的PC侧，CC=1n6时，是一个m2ts，n个clpi？)
*    +-DummySpace -- 占位文件，用于保证盘中有足够大的容量(1G?)
*
* CD-ROM(CD-R, CD-RW)
* 
* CF(CompactFlash)
*   Root
*    +-SONYCF.IND
* 
* DCF
*   DCF Folder命名规范
*       1.必须为8个字符；
*       2.第1个字符为数字1-9；
*       3.第2-3个字符为数字0-9；
*       4.第4-8个字符0-9，A-Z，a-z，_。
*   DCF File命名规范
*       1.必须为12个字符(包括扩展名）
*       2.第1-4个字符为0-9，A-Z，a-z，_；
*       3.第5-8个字符为数字0-9，并且不能全为0。
*   DCF目录编号方式
*       1.取得/DCIM、/MP_ROOT中编号最大的文件夹及其编号（a）
*       2.从1中找到的文件夹中取最大的文件编号（b）
*       3.导出时文件夹编号(c)=(a),第一个导出文件编号(d)=b+1
*         实现上(是否不标准？)：原文件名中自由部分可以不变，仅修改编号。eg: Cano0028.JPG\Betw0001.JPG
*       4.文件编号上限9999，文件夹编号上限999。文件夹不存在时，创建文件夹。文件夹内最大文件个数4000。
*       5.混合导出时的编号方法(按照导出顺序，递增编号)：
*         /DCIM/101MSDCF/DSC00001.JPG     
*         /DCIM/101MSDCF/DSC00002.JPG     
*         /MP_ROOT/101ANV01/MAV00003.MP4     
*         /DCIM/101MSDCF/DSC00004.JPG
*   Root--
*    +-AVF_INFO\ -- AVIndex 目录
*      +-AVIN0001.INP -- Property 信息
*      +-AVIN0001.INT -- Thumbnail Data
*    +-MP_ROOT\  -- 
*      +-100ANV01\  -- MP4文件
*        +-MA*xmmm.MP4
*        +-MAQxmmm.MP4
*    +-DCIM
*      +-100XXXnn\
*        +-XXXXnnnn.JPG -- JPEG Files
*        +-XXXXnnnn.MPG -- L-mode Files
*    
*  
* 几种不同的分类方式？
*   DVD的形式有 DVD-Video、DVD-VR、DVD+VR、AVCHD、BDAV、HDMV 等(见 enum DvdFormat )
*   媒体种类有DVD-ROM、DVD-R、DVD-RW、DVD+R、DVD+RW、DVD-RAM
*   有8cm和12cm两种直径(diameter)， DL(DoubleLayer双层？)  
* DVD -- 
*   Root
*    +-AUDIO_TS\  -- 预留给DVD-AUDIO的目录
*    +-VIDEO_TS\  -- 存储所有DVD-VIDEO（包括音频，视频，字幕等）所有数据的目录 
*      +-VTS_XX_X.VOB(Video OBjects) -- 视频目标文件。由视频、声音、字幕数据流组成
*      +-VTS_TS.IFO(InFOrmation) -- 基本的浏览信息文件。
*          IFO文件给DVD播放机提供浏览信息，包括章节的开始时间，伴音流、字幕位置等。
*          VOB文件是电影本身，而相应的IFO文件把组成电影的各个片段有机连接起来。
*      +-VTS_XX_X.IFO -- 其他浏览信息文件（按照 标题_序号 的方式命名，最多可以有99个标题，每个标题下最多10个文件）
*      +-VTS_TS.BUP(BackUP)--第一个备份文件。
*          BUP和IFO文件完全相同，是留在如果IFO文件出错的时候使用的备份文件。
*    +-VIDEO_RM\ -- 如果是DVD+VR的话有该目录，其下会有VIDEO_RM.DAT 或 VIDEO_RM.IFO 等文件？
*    +-DVD_RTAV\ -- 如果是DVD-VR的话有该目录，其下会有VR_MOVIE.VRO文件
*
* HDDisc
*   Root
*    +-DiscInfo.ini     -- 由DDM刻录？
*    +-VideoFiles\
*      +-CtdHDPj.xml
*      +-PlayList.vzpl
* 
* MemoryStick(Memory Stick Video File Format) 
*   Root
*     +-MEMSTICK.IND
* 
*****************************************************************************************************/

#endif //DIRECT_SHOW_DIRECTORY_H