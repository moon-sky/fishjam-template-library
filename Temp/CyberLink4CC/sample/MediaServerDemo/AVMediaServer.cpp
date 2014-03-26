#include "StdAfx.h"
#include "AVMediaServer.h"

#include <uhttp/io/File.h>
#include <uhttp/util/StringUtil.h>
#include <cybergarage/xml/Parser.h>
#include <cybergarage/upnp/media/server/Directory.h>
#include <cybergarage/upnp/media/server/directory/file/FileDirectory.h>
#include <cybergarage/upnp/media/server/object/format/GIFFormat.h>
#include <cybergarage/upnp/media/server/object/format/JPEGFormat.h>
#include <cybergarage/upnp/media/server/object/format/PNGFormat.h>
#include <cybergarage/upnp/media/server/object/format/ID3Format.h>
#include <cybergarage/upnp/media/server/object/format/MPEGFormat.h>

#include <fstream>

using namespace std;
using namespace uHTTP;
using namespace CyberXML;
using namespace CyberLink;


const char *CAVMediaServer::DEFAULT_PREFERENCE_FILENAME = "AVMediaServer.xml";

const char *CAVMediaServer::PREFERENCE_CYBERGARAGE = "CyberGarage";
const char *CAVMediaServer::PREFERENCE_MEDIASERVER = "MediaServer";
const char *CAVMediaServer::PREFERENCE_CONTENT_DIRECTORY = "ContentDirectory";
const char *CAVMediaServer::PREFERENCE_DIRECTORY_LIST = "DirectoryList";
const char *CAVMediaServer::PREFERENCE_DIRECTORY = "Directory";
const char *CAVMediaServer::PREFERENCE_NAME = "Name";
const char *CAVMediaServer::PREFERENCE_PATH = "Path";


CAVMediaServer::CAVMediaServer(IAVMediaServerCallback* pCallback)
{
	m_pCallback = pCallback;

    addPlugIn(new ID3Format());
    addPlugIn(new GIFFormat());
    addPlugIn(new JPEGFormat());
    addPlugIn(new PNGFormat());
    addPlugIn(new MPEGFormat());
}

CAVMediaServer::~CAVMediaServer(void)
{
}

void CAVMediaServer::DumpInfo()
{
	FTLTRACEA("MediaServer DumpInfo\n");
}

bool CAVMediaServer::loadPreferences(const char *fname)
{
    File file(fname);
    Parser xmlParser;
    Node *rootNode = xmlParser.parse(&file);
    if (rootNode == NULL)
        return false;

    if (rootNode->isName(PREFERENCE_CYBERGARAGE) == false)
        return false;

    Node *msNode = rootNode->getNode(PREFERENCE_MEDIASERVER);
    if (msNode == NULL)
        return false;

    Node *conNode = msNode->getNode(PREFERENCE_CONTENT_DIRECTORY);
    if (conNode == NULL)
        return false;

    Node *dirListNode = conNode->getNode(PREFERENCE_DIRECTORY_LIST);
    if (dirListNode == NULL)
        return false;

    int dirCnt = dirListNode->getNNodes();
    for (int n=0; n<dirCnt; n++) {
        Node *dirNode = dirListNode->getNode(n);
        if (dirNode->isName(PREFERENCE_DIRECTORY) == false)
            continue;
        Node *nameNode = dirNode->getNode(PREFERENCE_NAME);
        Node *pathNode = dirNode->getNode(PREFERENCE_PATH);
        if (nameNode == NULL || pathNode == NULL)
            continue;
        const char *name = nameNode->getValue();
        const char *path = pathNode->getValue();
#ifdef WIN32
        string pathStr = path;
        path = StringReplaceChars(pathStr, "'", "\\");
#endif
        FileDirectory *fileDir = new FileDirectory(name, path);
        addContentDirectory(fileDir);
    }

    getContentDirectory()->print();
    delete rootNode;

    return true;
}

bool CAVMediaServer::savePreferences(const char *fname)
{
    Node *rootNode = new Node(PREFERENCE_CYBERGARAGE);
    Node *msNode = new Node(PREFERENCE_MEDIASERVER);
    Node *conDirNode = new Node(PREFERENCE_CONTENT_DIRECTORY);
    Node *dirListNode = new Node(PREFERENCE_DIRECTORY_LIST);

    rootNode->addNode(msNode);
    msNode->addNode(conDirNode);
    conDirNode->addNode(dirListNode);

    int dirCnt = getNContentDirectories();
    for (int n=0; n<dirCnt; n++) {
        Node *dirNode = new Node(PREFERENCE_DIRECTORY);
        FileDirectory *dir = (FileDirectory *)getContentDirectory(n);
        Node *nameNode = new Node(PREFERENCE_NAME);
        nameNode->setValue(dir->getFriendlyName());
        Node *pathNode = new Node(PREFERENCE_PATH);
        pathNode->setValue(dir->getPath());
        dirListNode->addNode(dirNode);
        dirNode->addNode(nameNode);
        dirNode->addNode(pathNode);
    }

    string buf;
    //rootNode->toUnicodeString(buf);
    rootNode->toString(buf);

    fstream fout(fname, ios::out);
    if (fout.fail())
        return false;
    fout << XML::VERSION_HEADER << endl;
    fout << buf;
    fout.close();

    delete rootNode;

    return true;
}

bool CAVMediaServer::AddDirectory(const char* name, const char* path){
    FileDirectory *fileDir = new FileDirectory(name, path);
    addContentDirectory(fileDir);

    return true;
}