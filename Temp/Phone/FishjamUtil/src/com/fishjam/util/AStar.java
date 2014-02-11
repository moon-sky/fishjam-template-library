package com.fishjam.util;

import java.util.ArrayList;

import android.util.Log;

/**************************************************************************************************************************************
 * 游戏地图编辑器( http://download.csdn.net/download/xys289187120/3452259 )
 *   mappy(mapwin) -- 使用 playback library 来装载和显示FMP地图文件
 *   tiled
 * 术语：
 *   
 * 地图编辑器 -- http://fc00.deviantart.net/fs70/f/2011/034/c/c/tilemap_editor_b55_pokemon_by_digibody-d36t3aq.swf
 *
 * A星算法 -- 在图形平面上，有多个节点的路径，求出最低通过成本的算法
 *   http://blog.sina.com.cn/s/blog_53a5865c0102dycw.html
 *     其实现中 open-list和close-list使用的完全是静态数组，降低了入栈出栈的负担。最后获得的路径，是从终点指向起点的
 *     
 ***************************************************************************************************************************************/

/*
 * 
AStar asr = new AStar(map, start_x, start_y, end_x, end_y);
PosInfo[] result = asr.getResult();
if(result != null) { success } 
*/

//八方向寻路
public class AStar {
	private final static String TAG = AStar.class.getSimpleName();
	
	public class PosInfo {
		public PosInfo(int iRow, int iCol, int iDir) {
			this.row = iRow;
			this.col = iCol;
			this.dir = iDir;
		}

		public PosInfo(PosInfo posInfo) {
			this.row = posInfo.row;
			this.col = posInfo.col;
			this.dir = posInfo.dir;
		}

		public int row;
		public int col;
		public int dir;
	}

	private int[][] mMap; // 地图矩阵，0表示能通过，1表示不能通过
	private int mMapRowCount; 	// 地图宽度
	private int mMapColumnCount; 				// 地图高度
	private int mStartRow; 		// 起点行数
	private int mStartCol; 	// 起点列数
	private int mEndRow; 		// 终点行数
	private int mEndCol; 	// 终点列数
	private boolean mEnable8Way;		//是否允许8方向寻路，false表示4方向
	
	private boolean mCloseList[][]; // 关闭列表
	public int mOpenList[][][]; // 打开列表
	private int mOpenListLength;

	private static final int EXIST = 1;
	private static final int NOT_EXIST = 0;

	private static final int ISEXIST = 0;
	private static final int EXPENSE = 1; // 自身的代价
	private static final int DISTANCE = 2; // 距离的代价
	private static final int COST = 3; // 消耗的总代价
	private static final int FATHER_DIR = 4; // 父节点的方向

	public static final int DIR_NULL = 0;
	public static final int DIR_DOWN = 1; // 方向：下
	public static final int DIR_UP = 2; // 方向：上
	public static final int DIR_LEFT = 3; // 方向：左
	public static final int DIR_RIGHT = 4; // 方向：右
	public static final int DIR_UP_LEFT = 5;
	public static final int DIR_UP_RIGHT = 6;
	public static final int DIR_DOWN_LEFT = 7;
	public static final int DIR_DOWN_RIGHT = 8;

	private static final int ASTAR_DEPTH = 5000; // 算法嵌套深度
	private boolean isFound; // 是否找到路径

	public AStar(int[][] map, int startRow, int startColumn, int endRow, int endColumn, boolean bEnable8Way) {
		mStartRow = startRow;
		mStartCol = startColumn;
		mEndRow = endRow;
		mEndCol = endColumn;
		mEnable8Way = bEnable8Way;
		
		mMap = map;
		mMapRowCount = map.length;
		mMapColumnCount = map[0].length;
		
		Log.e(TAG, "AStar, Row=" + mMapRowCount +",Column=" + mMapColumnCount 
				+",canPasStart[" + mMap[mStartRow][mStartCol] + "]=" + checkPass(mStartRow, mStartCol)
				+",canPasGoal[" + mMap[mEndRow][mEndCol] + "]=" + checkPass(mEndRow, mEndCol)
		);
		
		initCloseList();
		initOpenList(mEndRow, mEndCol);
	}

	// 得到地图上这一点的消耗值
	private int getMapExpense(int row, int col, int dir) {
		if (dir < 5) {
			return 10;
		} else {
			return 14;
		}
	}

	// 得到距离的消耗值
	private int getDistance(int sRow, int sCol, int eRow, int eCol) {
		return 10 * (Math.abs(sRow - eRow) + Math.abs(sCol - eCol));
	}

	// 得到给定坐标格子此时的总消耗值
	private int getCost(int iRow, int iCol) {
		return mOpenList[iRow][iCol][COST];
	}

	// 开始寻路
	public void searchPath() {
		Log.e(TAG, "isCanPass Start=" + isCanPass(mStartRow, mStartCol));//+", Reason=" + sTmpReason);
		Log.e(TAG, "isCanPass Goal=" + isCanPass(mEndRow, mEndCol));// +", Reason=" + sTmpReason);
		
		if (isCanPass(mStartRow, mStartCol) && isCanPass(mEndRow, mEndCol)) {
			addOpenList(mStartRow, mStartCol);
			aStar(mStartRow, mStartCol);
		}
		String strFound = "" + isFound;
		Log.e(TAG, String.format("searchPath, [%1$d,%2$d] -> [%3$d,%4$d], isFound=%5$s", mStartRow, mStartCol, mEndRow, mEndCol, 
				strFound));
	}

	// 寻路
	private void aStar(int row, int col) {
		// 控制算法深度
		for (int depth = 0; depth < ASTAR_DEPTH; depth++) {
			if (((row == mEndRow) && (col == mEndCol))) {
				Log.i(TAG, "find AStart, depth=" + depth);
				isFound = true;
				return;
			} else if ((mOpenListLength == 0)) {
				Log.i(TAG, "can not find AStart, depth=" + depth);
				isFound = false;
				return;
			}

			removeOpenList(row, col);
			addCloseList(row, col);

			// 该点周围能够行走的点
			addNewOpenList(row, col, row, col + 1, DIR_UP);
			addNewOpenList(row, col, row, col - 1, DIR_DOWN);
			addNewOpenList(row, col, row - 1, col, DIR_RIGHT);
			addNewOpenList(row, col, row + 1, col, DIR_LEFT);
			if (mEnable8Way) {
				addNewOpenList(row, col, row + 1, col + 1, DIR_UP_LEFT);
				addNewOpenList(row, col, row - 1, col + 1, DIR_UP_RIGHT);
				addNewOpenList(row, col, row + 1, col - 1, DIR_DOWN_LEFT);
				addNewOpenList(row, col, row - 1, col - 1, DIR_DOWN_RIGHT);
			}

			// 找到估值最小的点，进行下一轮算法
			int cost = 0x7fffffff;
			for (int i = 0; i < mMapRowCount; i++) {
				for (int j = 0; j < mMapColumnCount; j++) {
					if (mOpenList[i][j][ISEXIST] == EXIST) {
						if (cost > getCost(i, j)) {
							cost = getCost(i, j);
							row = i;
							col = j;
						}
					}
				}
			}
		}
		// 算法超深
		Log.e(TAG, "Too big map, RowCount=" + mMapRowCount + ",ColCount" + mMapColumnCount);
		isFound = false;
		return;
	}

	// 添加一个新的节点
	private void addNewOpenList(int sRow, int sCol, int eRow, int eCol, int dir) {
		if (isCanPass(eRow, eCol)) {
			if (mOpenList[eRow][eCol][ISEXIST] == EXIST) {
				if (mOpenList[sRow][sCol][EXPENSE] + getMapExpense(eRow, eCol, dir) < mOpenList[eRow][eCol][EXPENSE]) {
					setFatherDir(eRow, eCol, dir);
					setCost(eRow, eCol, sRow, sCol, dir);
				}
			} else {
				addOpenList(eRow, eCol);
				setFatherDir(eRow, eCol, dir);
				setCost(eRow, eCol, sRow, sCol, dir);
			}
		}
	}

	// 设置消耗值
	private void setCost(int sRow, int sCol, int eRow, int eCol, int dir) {
		mOpenList[sRow][sCol][EXPENSE] = mOpenList[eRow][eCol][EXPENSE]
				+ getMapExpense(sRow, sCol, dir);
		mOpenList[sRow][sCol][DISTANCE] = getDistance(sRow, sCol, eRow, eCol);
		mOpenList[sRow][sCol][COST] = mOpenList[sRow][sCol][EXPENSE]
				+ mOpenList[sRow][sCol][DISTANCE];
	}

	// 设置父节点方向
	private void setFatherDir(int iRow, int iCol, int iDir) {
		mOpenList[iRow][iCol][FATHER_DIR] = iDir;
	}

	//可重载
	protected boolean checkPass(int iRow, int iCol){
		if (mMap[iRow][iCol] != 325) {
			return false;
		}
		return true;
	}
	
	//private String sTmpReason;
	// 判断一个点是否可以通过
	private boolean isCanPass(int iRow, int iCol) {
		// 超出边界
		if (iRow < 0 || iRow >= mMapRowCount || iCol < 0 || iCol >= mMapColumnCount) {
			//sTmpReason = "OverBounds, iRow=" + iRow + ",nCol=" + iCol 
			//		+ ",mMapRowCount=" + mMapRowCount +",mMapColumnCount=" + mMapColumnCount;
			return false;
		}
		
		// 地图不通
		if (!checkPass(iRow, iCol)) {
			//sTmpReason = "checkPass";
			return false;
		}
		// 在关闭列表中
		if (isInCloseList(iRow, iCol)) {
			//sTmpReason = "inCloseList, iRow=" + iRow + ",nCol=" + iCol;
			return false;
		}
		//sTmpReason = "";
		return true;
	}

	// 移除打开列表的一个元素
	private void removeOpenList(int iRow, int iCol) {
		if (mOpenList[iRow][iCol][ISEXIST] == EXIST) {
			mOpenList[iRow][iCol][ISEXIST] = NOT_EXIST;
			mOpenListLength--;
		}
	}

	// 判断一点是否在关闭列表中
	private boolean isInCloseList(int iRow, int iCol) {
		return mCloseList[iRow][iCol];
	}

	// 添加关闭列表
	private void addCloseList(int iRow, int iCol) {
		mCloseList[iRow][iCol] = true;
	}

	// 添加打开列表
	private void addOpenList(int iRow, int iCol) {
		if (mOpenList[iRow][iCol][ISEXIST] == NOT_EXIST) {
			mOpenList[iRow][iCol][ISEXIST] = EXIST;
			mOpenListLength++;
		}
	}

	// 初始化关闭列表
	private void initCloseList() {
		mCloseList = new boolean[mMapRowCount][mMapColumnCount];
		for (int row = 0; row < mMapRowCount; row++) {
			for (int col = 0; col < mMapColumnCount; col++) {
				mCloseList[row][col] = false;
			}
		}
	}

	// 初始化打开列表
	private void initOpenList(int endRow, int endCol) {
		mOpenList = new int[mMapRowCount][mMapColumnCount][5];
		for (int row = 0; row < mMapRowCount; row++) {
			for (int col = 0; col < mMapColumnCount; col++) {
				mOpenList[row][col][ISEXIST] = NOT_EXIST;
				mOpenList[row][col][EXPENSE] = getMapExpense(row, col, DIR_NULL);
				mOpenList[row][col][DISTANCE] = getDistance(row, col, endRow, endCol);
				mOpenList[row][col][COST] = mOpenList[row][col][EXPENSE] + mOpenList[row][col][DISTANCE];
				mOpenList[row][col][FATHER_DIR] = DIR_NULL;
			}
		}
		mOpenListLength = 0;
	}

	public String getDirString(int dir){
		switch (dir) {
		case DIR_NULL: 					return "N";
		case DIR_DOWN: 				return "↓";
		case DIR_UP:						return "↑";
		case DIR_LEFT:					return "←";
		case DIR_RIGHT:				return "→";
		case DIR_UP_LEFT:				return "↖";
		case DIR_UP_RIGHT:			return "↗";
		case DIR_DOWN_LEFT:		return "↙";
		case DIR_DOWN_RIGHT:		return "↘";
		default:									return "Unknown";
		}
	}
	// 获得寻路结果
	public PosInfo[] getResult(boolean includeStart) {
		PosInfo[] result;
		ArrayList<PosInfo> route;
		searchPath();
		if (!isFound) {
			return null;
		}
		route = new ArrayList<PosInfo>();
		// openList是从目标点向起始点倒推的。
		int iX = mEndRow;
		int iY = mEndCol;
		while ((iX != mStartRow || iY != mStartCol)) {
			route.add(new PosInfo(iX, iY, mOpenList[iX][iY][FATHER_DIR]));
			switch (mOpenList[iX][iY][FATHER_DIR]) {
			case DIR_DOWN:
				iY++;
				break;
			case DIR_UP:
				iY--;
				break;
			case DIR_LEFT:
				iX--;
				break;
			case DIR_RIGHT:
				iX++;
				break;
			case DIR_UP_LEFT:
				iX--;
				iY--;
				break;
			case DIR_UP_RIGHT:
				iX++;
				iY--;
				break;
			case DIR_DOWN_LEFT:
				iX--;
				iY++;
				break;
			case DIR_DOWN_RIGHT:
				iX++;
				iY++;
				break;
			}
		}
		if (includeStart) {
			route.add(new PosInfo(mStartRow, mStartCol, mOpenList[mStartRow][mStartCol][FATHER_DIR]));
		}
		int size = route.size();
		result = new PosInfo[size];
		for (int i = 0; i < size; i++) {
			result[i] = new PosInfo((PosInfo) route.get(i));
		}
		return result;
	}
}