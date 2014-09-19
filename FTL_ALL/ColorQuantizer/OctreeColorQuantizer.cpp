#include "stdafx.h"
#include "OctreeColorQuantizer.h"
#include <ftlGdi.h>
#include <algorithm>

namespace FTL
{
    BYTE CFOctreeNode::s_MASK[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

    CFOctreeNode::CFOctreeNode(int level, CFOctreeColorQuantizer* pParent)
    {
        FTLASSERT(level <= 7);

        m_Level = level;
        m_Red = 0;
        m_Green = 0;
        m_Blue = 0;
        m_nPixelCount = 0;
        m_nPaletteIndex = -1;
        ZeroMemory(m_pNodes, sizeof(CFOctreeNode*) * _countof(m_pNodes));

        if (level < 7)
        {
            pParent->AddLevelNode(level, this);
        }
        else{
            FTLASSERT(level == 7);
        }
    }

    CFOctreeNode::~CFOctreeNode()
    {
        for (int i = 0; i < _countof(m_pNodes); i++)
        {
            if (NULL != m_pNodes[i])
            {
                SAFE_DELETE(m_pNodes[i]);
            }
        }
    }
     
    BOOL CFOctreeNode::IsLeaf() const
    { 
        return m_nPixelCount > 0; 
    }

    COLORREF CFOctreeNode::GetColor() const
    {
        COLORREF result = RGB(0, 0, 0);

        // determines a color of the leaf
        if (IsLeaf())
        {
#ifdef _DEBUG
            int nRet = m_Red / m_nPixelCount;
            int nGreen = m_Green / m_nPixelCount;
            int nBlue = m_Blue / m_nPixelCount;
            FTLASSERT((nRet >=0 && nRet <= 255)
                && ( nGreen >= 0 && nGreen <= 255)
                && ( nBlue >=0 && nBlue <= 255));
#endif 
            result = (m_nPixelCount == 1 ? 
                MAKE_RGBA(m_Red, m_Green, m_Blue, 0xFF) : 
                MAKE_RGBA(m_Red/m_nPixelCount, m_Green/m_nPixelCount, m_Blue/m_nPixelCount, 0xFF));
        }
        else
        {
            FTLASSERT(FALSE && TEXT("Cannot retrieve a color for other node than leaf."));
        }

        return result;
    }

    int CFOctreeNode::GetActiveNodesPixelCount() const
    {
        int result = m_nPixelCount;

        // sums up all the pixel presence for all the active nodes
        for (int index = 0; index < _countof(m_pNodes); index++)
        {
            CFOctreeNode* pNode = m_pNodes[index];

            if (pNode != NULL)
            {
                result += pNode->m_nPixelCount;
            }
        }

        return result;
    }

    void CFOctreeNode::GetActiveNodes(OctreeNodeList& result) const
    {
        // adds all the active sub-nodes to a list
        for (int index = 0; index < _countof(m_pNodes); index++)
        {
            CFOctreeNode* pNode = m_pNodes[index];

            if (pNode != NULL)
            {
                if (pNode->IsLeaf())
                {
                    result.push_back(pNode);
                }
                else
                {
                    pNode->GetActiveNodes(result);
                }
            }
        }
    }

    void CFOctreeNode::AddColor(COLORREF color, int level, CFOctreeColorQuantizer* pParent)
    {
        // if this node is a leaf, then increase a color amount, and pixel presence
        if (level == 8)
        {
            m_Red += GetRValue(color);
            m_Green += GetGValue(color);
            m_Blue += GetBValue(color);
            m_nPixelCount++;
        }
        else if (level < 8) // otherwise goes one level deeper
        {
            // calculates an index for the next sub-branch
            int index = GetColorIndexAtLevel(color, level);

            // if that branch doesn't exist, grows it
            if (m_pNodes[index] == NULL)
            {
                m_pNodes[index] = new CFOctreeNode(level, pParent);
            }

            // adds a color to that branch
            m_pNodes[index]->AddColor(color, level + 1, pParent);
        }
    }

    int CFOctreeNode::GetPaletteIndex(COLORREF color, int level)
    {
        int result = 0;

        // if a node is leaf, then we've found are best match already
        if (IsLeaf())
        {
            result = m_nPaletteIndex;
            FTLASSERT(result != -1);
        }
        else // otherwise continue in to the lower depths
        {
#if 1
            int index = GetColorIndexAtLevel(color, level);
            CFOctreeNode* pNode = m_pNodes[index];
            if (pNode != NULL)
            {
                result = pNode->GetPaletteIndex(color, level + 1);
            }
            else
            {
                for (int i = 0; i < 8; i++)
                {
                    if (m_pNodes[i] != NULL)
                    {
                        result = m_pNodes[i]->GetPaletteIndex(color, level + 1);
                        break;
                    }
                }
            }
#endif 
        }

        return result;
    }

    int CFOctreeNode::RemoveLeaves(int level, int activeColorCount, int targetColorCount, CFOctreeColorQuantizer* parent)
    {
        int result = 0;

        // scans thru all the active nodes
        for (int index = 0; index < _countof(m_pNodes); index++)
        {
            CFOctreeNode* pNode = m_pNodes[index];

            if (pNode != NULL)
            {
                // sums up their color components
                m_Red += pNode->m_Red;
                m_Green += pNode->m_Green;
                m_Blue += pNode->m_Blue;

                // and pixel presence
                m_nPixelCount += pNode->m_nPixelCount;

                // increases the count of reduced nodes
                result++;

                SAFE_DELETE(m_pNodes[index]);
            }
        }

        // returns a number of reduced sub-nodes, minus one because this node becomes a leaf
        return result - 1;
    }

    int CFOctreeNode::GetColorIndexAtLevel(COLORREF color, int level)
    {
        return ((GetRValue(color) & s_MASK[level]) == s_MASK[level] ? 4 : 0) |
            ((GetGValue(color) & s_MASK[level]) == s_MASK[level] ? 2 : 0) |
            ((GetBValue(color) & s_MASK[level]) == s_MASK[level] ? 1 : 0);
    }

    void CFOctreeNode::SetPaletteIndex(int index)
    {
        m_nPaletteIndex = index;
    }
    /*************************************************************************************************
    * 
    *************************************************************************************************/

    CFOctreeColorQuantizer::CFOctreeColorQuantizer()
    {
        //m_nLastColorCount = 0;
        m_nLevelNodeCount = 0;
        m_pRoot = NULL;
        FTLASSERT(sizeof(m_pLevels) == 28); // sizeof(OctreeNodeList*) * 7
        for (int i = 0; i < _countof(m_pLevels); i++)
        {
            m_pLevels[i] = new OctreeNodeList();
        }
    }
    CFOctreeColorQuantizer::~CFOctreeColorQuantizer()
    {
        SAFE_DELETE(m_pRoot);
        for (int i = 0; i < _countof(m_pLevels); i++)
        {
            //m_pLevels[i]->clear();
            SAFE_DELETE(m_pLevels[i]);
        }
    }

    BOOL CFOctreeColorQuantizer::OnPrepare()
    {
        BOOL bRet = FALSE;
        
        API_VERIFY(__super::OnPrepare());
        API_VERIFY(_AnalyzeColorMeta());

        // creates a root node
        m_pRoot = new CFOctreeNode(0, this);

        for (ColorMetaList::iterator iter = m_clrList.begin(); 
            iter != m_clrList.end();
            ++iter)
        {
            m_pRoot->AddColor(*iter, 0, this);
        }

        return bRet;
    }

    BOOL CFOctreeColorQuantizer::OnProcessQuantizer(UINT nWantClrCount, UINT *pResultClrCount)
    {
        FUNCTION_BLOCK_NAME_TRACE(TEXT("OnProcessQuantizer"), 100);
        // use optimized palette, if any
#if 0
        List<Color> optimizedPalette = base.OnGetPalette(colorCount);
        if (optimizedPalette != null) return optimizedPalette;
#endif 
        // otherwise let's get to build one
        //std::list<COLORREF> result;

        INT paletteIndex = 0;
        INT nResultCount = 0;
        INT leafCount = 0;
        {
            FUNCTION_BLOCK_NAME_TRACE(TEXT("GetLeaves"), 100);
            OctreeNodeList curLeaves;
            leafCount = Leaves(curLeaves);
            //m_nLastColorCount = leafCount;
        }

        {
            FUNCTION_BLOCK_NAME_TRACE(TEXT("GoThroughAllLevels"), 100);
            // goes thru all the levels starting at the deepest, and goes upto a root level
            for (int level = 6; level >= 0; level--)
            {
                // if level contains any node
                if (m_pLevels[level]->size() > 0)
                {
                    // orders the level node list by pixel presence (those with least pixels are at the top)
                    OctreeNodeList& sortedNodeList = *m_pLevels[level];
                    //IEnumerable<OctreeNode> sortedNodeList = levels[level].OrderBy(node => node.ActiveNodesPixelCount);
                    sortedNodeList.sort(FTL::UnreferenceLess<CFOctreeNode*>());

                    // removes the nodes unless the count of the leaves is lower or equal than our requested color count
                    for(OctreeNodeList::iterator iter = sortedNodeList.begin();
                        iter != sortedNodeList.end(); 
                        ++iter)
                    {
                        CFOctreeNode* pNode = (*iter);
                        // removes a node
                        leafCount -= pNode->RemoveLeaves(level, leafCount, nWantClrCount, this);

                        // if the count of leaves is lower then our requested count terminate the loop
                        if (leafCount <= nWantClrCount){
                            break;
                        }
                    }

                    // if the count of leaves is lower then our requested count terminate the level loop as well
                    if (leafCount <= nWantClrCount){
                        break;
                    }
                    // otherwise clear whole level, as it is not needed anymore
                    m_pLevels[level]->clear();
                }
            }
        }

        {
            FUNCTION_BLOCK_TRACE(100);
            // goes through all the leaves that are left in the tree (there should now be less or equal than requested)
            OctreeNodeList sortLeaves;
            Leaves(sortLeaves);

            SAFE_DELETE_ARRAY(m_pResultPalette);
            m_pResultPalette = new COLORREF[nWantClrCount];
            ZeroMemory(m_pResultPalette, sizeof(COLORREF) * nWantClrCount);

            sortLeaves.sort(FTL::UnreferenceLess<CFOctreeNode*>());
            for(OctreeNodeList::reverse_iterator iter = sortLeaves.rbegin(); 
                iter != sortLeaves.rend();
                iter++)
                //foreach (OctreeNode node in Leaves.OrderByDescending(node => node.ActiveNodesPixelCount))
            {
                CFOctreeNode* pNode = *iter;

                // adds the leaf color to a palette
                if (pNode->IsLeaf())
                {
                    m_pResultPalette[nResultCount++] = pNode->GetColor();
                }

                // and marks the node with a palette index
                pNode->SetPaletteIndex(paletteIndex++);

                if (paletteIndex >= nWantClrCount)
                {
                    break;
                }
            }

            // we're unable to reduce the Octree with enough precision, and the leaf count is zero
            if (nResultCount == 0)
            {
                FTLASSERT(FALSE && "The Octree contains after the reduction 0 colors, it may happen for 1-16 colors because it reduces by 1-8 nodes at time. Should be used on 8 or above to ensure the correct functioning.");
            }
            if (pResultClrCount)
            {
                *pResultClrCount = nResultCount;
            }
            m_nPaletteItemCount = nResultCount;
        }

        {
            FUNCTION_BLOCK_NAME_TRACE(TEXT("GetResultPaletteIndex"), 100);
            FTLASSERT(m_clrList.size() == m_nHeight * m_nWidth);

            m_QuantizerResultIndexes.resize(m_nHeight * m_nWidth);
            int palIndex = 0;
            for (ColorMetaList::iterator iter = m_clrList.begin();
                iter != m_clrList.end();
                ++iter)
            {
                int nPaletteIndex = m_pRoot->GetPaletteIndex(*iter, 0);
                FTLASSERT(0 <= nPaletteIndex && nPaletteIndex <= UCHAR_MAX);
                m_QuantizerResultIndexes[palIndex++] = (UCHAR)nPaletteIndex;
            }
        }

        return nResultCount;
    }

    void CFOctreeColorQuantizer::OnFinish()
    {
    }

    INT CFOctreeColorQuantizer::Leaves(OctreeNodeList& result)
    {
#if 1
        OctreeNodeList activeNodes;
        m_pRoot->GetActiveNodes(activeNodes);
        for (OctreeNodeList::iterator iter = activeNodes.begin(); 
            iter != activeNodes.end();
            ++iter)
        {
            if ((*iter)->IsLeaf())
            {
                //OctreeNodeListnsertResult insertResult = 
                result.push_back(*iter);
                //FTLASSERT(insertResult.second);
            }
        }
        FTLASSERT(activeNodes.size() == result.size());
        result.clear();
#endif
        m_pRoot->GetActiveNodes(result);
        return result.size();
        //return root.ActiveNodes.Where(node => node.IsLeaf); 
    }

    void CFOctreeColorQuantizer::AddLevelNode(int level, CFOctreeNode* octreeNode)
    { 
        m_nLevelNodeCount++;
        m_pLevels[level]->push_back(octreeNode); 
    }
}

