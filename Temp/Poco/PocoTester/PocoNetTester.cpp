#include "StdAfx.h"
#include "PocoNetTester.h"

#include "Poco/Net/HTTPSessionFactory.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/URI.h"
void CPocoNetTester::test_container_holder()
{
}

void CPocoNetTester::test_PocoHttp()
{
    std::string urlString = "http://dev.apis.naver.com/naverMemo/memo/sync.json?command=Provision&userId=fishjam2000@naver.com&deviceId=DESKTOP8c89a53d6dd7&msgpad=1393491827000&md=lX9wvPIYrqNH8czc9OFuDQdqMv4%3D";
    std::string contents = "";
    typedef std::map<std::string, std::string> HEADERS;
    HEADERS headers;
    typedef Poco::Net::NameValueCollection Cookies;
    Cookies* cookies = NULL;

    headers["User-Agent"] = "nApps (windows 7; desktop; NaverMemo; 0.9.23.124)";
    headers["Accept-Encoding"] = "gzip";


    Poco::URI uri(urlString);

    std::string path(uri.getPathAndQuery());
    if (path.empty())
    {
        path = "/";
    }

    Poco::Net::HTTPResponse::HTTPStatus status = Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR;
    try
    {
        std::tr1::shared_ptr<Poco::Net::HTTPClientSession> session;
        session = std::tr1::shared_ptr<Poco::Net::HTTPClientSession>
            (Poco::Net::HTTPSessionFactory::defaultFactory().createClientSession(uri));
        session->setTimeout(Poco::Timespan(10, 0));
        Poco::Net::HTTPRequest request(
            contents.empty() ? Poco::Net::HTTPRequest::HTTP_GET : Poco::Net::HTTPRequest::HTTP_POST, 
            path, Poco::Net::HTTPMessage::HTTP_1_1);
        request.setContentType("application/x-www-form-urlencoded");
        request.setKeepAlive(true);

        for(HEADERS::iterator header = headers.begin(); 
            header != headers.end(); 
            ++header)
        {
            request.add(header->first, header->second);
        }

        if (cookies)
        {
            //setCookies(request, *cookies);
            request.setCookies(*cookies);
        }

        request.setContentLength( (int) contents.length());
        session->sendRequest(request) << contents;
    }

    catch( const std::exception& e )
    {
        OutputDebugStringA(e.what());
    }

    status == Poco::Net::HTTPResponse::HTTP_OK ? true:false;
}