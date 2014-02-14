package com.fishjam.storehelper;


public class CommonFunction {
	/*
	public interface HTTPCallBack {  
	    void execute(String str, int id, boolean thread);  
	}   
	
	public static void dispatchSwitchResult(Activity activity, Intent data)
	{
		Intent intent = new Intent();
	    
		int index = data.getIntExtra("SWITCH_RESULT", SwitchPanelActivity.SWITCH_RESULT_CODE_QUICKSEARCH);
		switch(index)
		{
		case SwitchPanelActivity.SWITCH_RESULT_CODE_QUICKSEARCH:			
			intent.setClassName(activity, NaverSearchActivity.class.getName());
		    intent.putExtra("search", data.getStringExtra("search"));				    
		    break;
		case SwitchPanelActivity.SWITCH_RESULT_CODE_SEARCH:
			if(activity instanceof NaverSearchActivity)
				return;
			intent.setClassName(activity, NaverSearchActivity.class.getName());		   
			break;
		case SwitchPanelActivity.SWITCH_RESULT_CODE_QR:
			if(activity instanceof QRCodeActivity)
				return;
			intent.setClassName(activity, QRCodeActivity.class.getName());
			activity.finish();
			AEONMainActivity.getInstance().startActivityForResult(intent, 0);

			return;
		case SwitchPanelActivity.SWITCH_RESULT_CODE_SHOPPING:
			if(activity instanceof PointShoppingListActivity)
				return;
			intent.setClassName(activity, PointShoppingListActivity.class.getName());
		    break;
		case SwitchPanelActivity.SWITCH_RESULT_CODE_MAP:
			if(activity instanceof AEONMapActivity)
				return;			
			intent.setClassName(activity, AEONMapActivity.class.getName());
			intent.putExtra("browsingUrl", "http://maps.google.com/");					
		    break;
		case SwitchPanelActivity.SWITCH_RESULT_CODE_SETTING:
			if(activity instanceof SettingActivity)
				return;
			intent.setClassName(activity, SettingActivity.class.getName());		    
			break;
		default:
			break;
		}
		activity.finish();
		AEONMainActivity.getInstance().startActivity(intent);		
	}

	public static void openSwitchResult(Activity activity) {
		// TODO Auto-generated method stub
		Intent intent = new Intent(activity, SwitchPanelActivity.class);   
		activity.startActivityForResult(intent, 0);   
		activity.overridePendingTransition(android.R.anim.slide_in_left,   
				android.R.anim.slide_out_right);
	}

	private static class HTTPRunnable implements Runnable{		
		private Activity mActivity;	
		private HTTPCallBack mCallback;
		private int mRequestId;
		public HTTPRunnable(int id, Activity activity, HTTPCallBack callback)
		{
			mActivity = activity;
			mCallback = callback;
			mRequestId = id;
		}
		
        @Override
        public void run() {
                        
        	String response = implementSendHTTP(mActivity, mRequestId);
        	if(mCallback != null)
        		mCallback.execute(response, mRequestId, true);
        }
    }
	
	private static String composeRequestUrl(int requestType) {
		// TODO Auto-generated method stub
		String url = AEONHttpClient.AEON_SERVER_URL;
		String vid = LocalInfo.getInstance().getVID();
		String tid = LocalInfo.getInstance().getTID();
		String ts = LocalInfo.getInstance().getTimeStamp();
		String ua = AEONHttpClient.AEON_UA;
		String m, word;
		
		switch(requestType)
		{
		case AEONHttpClient.AEON_HTTP_GET_INSTALL:
			m = AEONHttpClient.AEON_METHOD_INSTALL;
			return url + "/?TID="+tid+"&ts="+ts+"&m="+m;
		case AEONHttpClient.AEON_HTTP_GET_QUERY:
			m = AEONHttpClient.AEON_METHOD_QUERY;
			return url + "/?VID="+vid+"&TID="+tid+"&ts="+ts+"&m="+m;			
		case AEONHttpClient.AEON_HTTP_GET_SEARCH:
			m = AEONHttpClient.AEON_METHOD_SEARCH;
			word = LocalInfo.getInstance().getSearchWord();
			String type = LocalInfo.getInstance().getSearchType();
			return url + "/?VID="+vid+"&TID="+tid+"&ts="+ts+"&m="+m+"&kw="+word+"&sm="+type+"&ua="+ua;
		case AEONHttpClient.AEON_HTTP_GET_LOGIN:
			m = AEONHttpClient.AEON_METHOD_LOGIN;
			return "";
		case AEONHttpClient.AEON_HTTP_GET_LOGOUT:
			m = AEONHttpClient.AEON_METHOD_LOGOUT;
			return url + "/?VID="+vid+"&TID="+tid+"&ts="+ts+"&m="+m;
		case AEONHttpClient.AEON_HTTP_GET_SHOPLIST:
			m = AEONHttpClient.AEON_METHOD_SHOPLIST;
			word = LocalInfo.getInstance().getSearchWord();
			return url + "/?VID="+vid+"&TID="+tid+"&ts="+ts+"&m="+m+"&kw="+word+"&ua="+ua;
		case AEONHttpClient.AEON_HTTP_POST_USEDTIME:
			String ut = LocalInfo.getInstance().getUseTime();
			return url + "/?VID="+vid+"&TID="+tid+"&ts="+ts+"&ut="+ut+"&ua="+ua;
		case AEONHttpClient.AEON_HTTP_POST_GPS:
			Location loc = LocalInfo.getInstance().getGPSLocation();			
			String lng = ((loc == null) ? "" : ""+loc.getLongitude());
			String lat = ((loc == null) ? "" : ""+loc.getLatitude());
			return url + "/?VID="+vid+"&TID="+tid+"&ts="+ts+"&longitude="+lng+"&latitude="+lat+"&ua="+ua;
		case AEONHttpClient.AEON_HTTP_POST_NETTYPE:
			String nt = LocalInfo.getInstance().getNetworkType();
			return url + "/?VID="+vid+"&TID="+tid+"&ts="+ts+"&nt="+nt+"&ua="+ua;
		case AEONHttpClient.AEON_HTTP_POST_LOG:
			m = AEONHttpClient.AEON_METHOD_LOG;
			return url + "/?VID="+vid+"&TID="+tid+"&ts="+ts+"&m="+m;
		default:
			return "";
		}
	}
	
	private static Map<String, String> composeRequestEntity() {
		// TODO Auto-generated method stub
		String visitUrl = LocalInfo.getInstance().getVisitUrl();
		String vid = LocalInfo.getInstance().getVID();
		String tid = LocalInfo.getInstance().getTID();
		String ts = LocalInfo.getInstance().getTimeStamp();
		
		String tz = LocalInfo.getInstance().getCurrentZone(); 
		String lc = LocalInfo.getInstance().getCurrentLang();
		String rf = LocalInfo.getInstance().getRefer();
		String title = LocalInfo.getInstance().getWebTitle();
		String actType = LocalInfo.getInstance().getActType();		
		
		Map<String, String> paramsMap = new HashMap<String, String>();  
        paramsMap.put("u", visitUrl); 
        paramsMap.put("pz", ts); 
        paramsMap.put("tz", tz); 
        paramsMap.put("ref", rf); 
        paramsMap.put("ti", tid); 
        paramsMap.put("vid", vid); 
        paramsMap.put("sb", title); 
        paramsMap.put("act", actType); 
        paramsMap.put("lc", lc); 
        
        return paramsMap;
	}

	public static String implementSendHTTP(Activity activity, int requestType)
	{
		String url = composeRequestUrl(requestType);
		Map<String, String> paramsMap = null;
		if(requestType == AEONHttpClient.AEON_HTTP_POST_LOG)
			paramsMap = composeRequestEntity();
		try {
			// 使用工具类直接发出POST请求,服务器返回json数据，比如"{userid:12}"
			if(requestType >= AEONHttpClient.AEON_HTTP_POST_USEDTIME)
				return AEONHttpClient.post(url, paramsMap, null);
			else
				return AEONHttpClient.get(url, null);
		}catch (RuntimeException e) {
			// 请求失败或者连接失败
			return null;
		}
	}

	public static boolean sendHTTPRequest(int requestId, Activity activity, boolean isThread, HTTPCallBack callback) {
		// TODO Auto-generated method stub
		if(isThread)
		{
			ThreadPoolUtils.execute(new HTTPRunnable(requestId, activity, callback));
		}else
		{
			String response = implementSendHTTP(activity, requestId);
			if(callback != null)
				callback.execute(response, requestId, false);		
		}
		
		return true;
	}	
	
	public static String convertToString(InputStream inputStream){
        StringBuffer string = new StringBuffer();
        BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream));
        String line;
        try {
            while ((line = reader.readLine()) != null) {
                string.append(line + "\n");
            }
        } catch (IOException e) {}
        return string.toString();
    }

	final static String digits = "0123456789ABCDEF";

	public static String encode(String s) {
		// Guess a bit bigger for encoded form
		StringBuilder buf = new StringBuilder(s.length() + 16);
		for (int i = 0; i < s.length(); i++) {
			char ch = s.charAt(i);
			if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
					|| (ch >= '0' && ch <= '9') || ".-*_".indexOf(ch) > -1) { //$NON-NLS-1$  
				buf.append(ch);
			} else {
				byte[] bytes = new String(new char[] { ch }).getBytes();
				for (int j = 0; j < bytes.length; j++) {
					buf.append('%');
					buf.append(digits.charAt((bytes[j] & 0xf0) >> 4));
					buf.append(digits.charAt(bytes[j] & 0xf));
				}
			}
		}
	
		return buf.toString();
	}
	
	public static void loadWebPageWithReferer2(WebView webView, String urlConnection)
	{
		URL url;
        URLConnection conexion;
        try {
            url = new URL(urlConnection);
            conexion = url.openConnection();
            conexion.setConnectTimeout(3000);
            conexion.connect();
            // get the size of the file which is in the header of the request
            int size = conexion.getContentLength();
        } catch (Exception e) {}


        // and here, if you want, you can load the page normally
        String htmlContent = "";
        HttpGet httpGet = new HttpGet(urlConnection);
        // this receives the response
        String originalUrl = webView.getOriginalUrl();
		
        httpGet.setHeader("Referer", originalUrl);
        HttpResponse response;
        try {
        	
            response = AEONHttpClient.getHttpClient().execute(httpGet);
            if (response.getStatusLine().getStatusCode() == 200) {
                // la conexion fue establecida, obtener el contenido
            	Header[] h= response.getAllHeaders();
                HttpEntity entity = response.getEntity();
                if (entity != null) {
                    InputStream inputStream = entity.getContent();
                    htmlContent = convertToString(inputStream);
                }
            }
         } catch (Exception e) {}

         webView.loadData(encode(htmlContent), "text/html", "utf-8");
	}
	
	public static void loadWebPage(WebView webView, String url)
	{
		webView.loadUrl(url);
	}
	
	public static void loadWebPageWithReferer(WebView webView, String url)
	{
		String originalUrl = webView.getOriginalUrl();
		
		Map<String, String> additionalHttpHeaders = new HashMap<String, String>();
		additionalHttpHeaders.put("Referer", originalUrl);        
		LocalInfo.getInstance().setRefer(originalUrl);
		webView.loadUrl(url, additionalHttpHeaders);
	}
	
	*/
}
