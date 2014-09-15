#include "stdafx.h"
#include "OctreeColorQuantizer.h"
#include <ftlGdi.h>
#include <ftlFunctional.h>

namespace FTL
{
    BYTE CFOctreeNode::s_MASK[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
    CFOctreeNode::CFOctreeNode(int level, CFOctreeColorQuantizer* pParent)
    {
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

    int CFOctreeNode::GetActiveNodes(OctreeNodeList& result) const
    {
        int nCount= 0;
        // adds all the active sub-nodes to a list
        for (int index = 0; index < _countof(m_pNodes); index++)
        {
            CFOctreeNode* pNode = m_pNodes[index];

            if (pNode != NULL)
            {
                if (pNode->IsLeaf())
                {
                    result.push_back(pNode);
                    nCount++;
                }
                else
                {
                    nCount += pNode->GetActiveNodes(result);
                }
            }
        }

        return nCount;
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
            //result = nodes[index] != NULL ? nodes[index].GetPaletteIndex(color, level + 1) : nodes.
            //    Where(node => node != NULL).
            //    First().
            //    GetPaletteIndex(color, level + 1);
            //FTLASSERT(FALSE && TEXT("TODO"));
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
        m_nLastColorCount = 0;
        m_pRoot = NULL;
        m_pLevels = NULL;
    }
    CFOctreeColorQuantizer::~CFOctreeColorQuantizer()
    {
        SAFE_DELETE(m_pRoot);
        SAFE_DELETE_ARRAY(m_pLevels);
    }

    //void CFOctreeColorQuantizer::OnAddColor(Color color, Int32 key, Int32 x, Int32 y){
    //    root->AddColor(Color, 0, this);
    //}
    
    BOOL CFOctreeColorQuantizer::OnPrepare()
    {
        BOOL bRet = __super::OnPrepare();

        m_pLevels = new OctreeNodeList[7];

        // creates the octree level lists
        //for (int level = 0; level < 7; level++)
        //{
        //    levels[level] = new List<OctreeNode>();
        //}

        // creates a root node
        m_pRoot = new CFOctreeNode(0, this);

        int nPixOffset = (m_nBpp / 8);
        BYTE* pBuf = m_pBmpData;
        for (UINT h = 0; h < m_nHeight; h++)
        {
            for (UINT w = 0; w < m_nWidth; w++)
            {
                int Red = *(pBuf + 2);
                int Green = *(pBuf+1);
                int Blue = *(pBuf);

                COLORREF color = MAKE_RGBA(Red, Green, Blue, 0xFF);
                m_pRoot->AddColor(color, 0, this);
                pBuf += nPixOffset;
            }
        }
        //TODO: OnFinish();
        
        return bRet;
    }

    BOOL CFOctreeColorQuantizer::OnProcessQuantizer(UINT colorCount, UINT *pResultClrCount)
    {
        // use optimized palette, if any
#if 0
        List<Color> optimizedPalette = base.OnGetPalette(colorCount);
        if (optimizedPalette != null) return optimizedPalette;
#endif 
        // otherwise let's get to build one
        //std::list<COLORREF> result;
        SAFE_DELETE_ARRAY(m_pResultPalette);
        m_pResultPalette = new COLORREF[colorCount];
        ZeroMemory(m_pResultPalette, sizeof(COLORREF) * colorCount);

        OctreeNodeList curLeaves;
        
        int leafCount = Leaves(curLeaves);
        m_nLastColorCount = leafCount;
        int paletteIndex = 0;

        // goes thru all the levels starting at the deepest, and goes upto a root level
        for (int level = 6; level >= 0; level--)
        {
            // if level contains any node
            if (m_pLevels[level].size() > 0)
            {
                // orders the level node list by pixel presence (those with least pixels are at the top)
                OctreeNodeList sortedNodeList = m_pLevels[level];
                //IEnumerable<OctreeNode> sortedNodeList = levels[level].OrderBy(node => node.ActiveNodesPixelCount);
                sortedNodeList.sort(FTL::UnreferenceLess<CFOctreeNode*>());

                // removes the nodes unless the count of the leaves is lower or equal than our requested color count
                for(OctreeNodeList::iterator iter = sortedNodeList.begin();
                    iter != sortedNodeList.end(); 
                    ++iter)
                {
                    // removes a node
                    leafCount -= (*iter)->RemoveLeaves(level, leafCount, colorCount, this);

                    // if the count of leaves is lower then our requested count terminate the loop
                    if (leafCount <= colorCount){
                        break;
                    }
                }

                // if the count of leaves is lower then our requested count terminate the level loop as well
                if (leafCount <= colorCount){
                    break;
                }
                // otherwise clear whole level, as it is not needed anymore
                m_pLevels[level].clear();
            }
        }

        // goes through all the leaves that are left in the tree (there should now be less or equal than requested)
        OctreeNodeList sortLeaves;
        Leaves(sortLeaves);
        sortLeaves.sort(FTL::UnreferenceLess<CFOctreeNode*>());
        int nResultCount = 0;
        for(OctreeNodeList::reverse_iterator iter = sortLeaves.rbegin(); 
            iter != sortLeaves.rend();
            iter++)
        //foreach (OctreeNode node in Leaves.OrderByDescending(node => node.ActiveNodesPixelCount))
        {
            if (paletteIndex >= colorCount) break;

            // adds the leaf color to a palette
            if ((*iter)->IsLeaf())
            {
                m_pResultPalette[nResultCount++] = (*iter)->GetColor();
            }

            // and marks the node with a palette index
            (*iter)->SetPaletteIndex(paletteIndex++);
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

        int palIndex = 0;
        int nPixOffset = (m_nBpp / 8);
        BYTE* pBuf = m_pBmpData;
        m_indices.resize(m_nHeight * m_nWidth);
        for (UINT h = 0; h < m_nHeight; h++)
        {
            for (UINT w = 0; w < m_nWidth; w++)
            {
                int Red = *(pBuf + 2);
                int Green = *(pBuf+1);
                int Blue = *(pBuf);

                COLORREF color = MAKE_RGBA(Red, Green, Blue, 0xFF);
                
                m_indices[palIndex++] = m_pRoot->GetPaletteIndex(color, 0);
                pBuf += nPixOffset;
            }
        }

        return nResultCount;
    }

    //protected override void OnGetPaletteIndex(Color color, Int32 key, Int32 x, Int32 y, out Int32 paletteIndex)
    //{
    //    // retrieves a palette index
    //    paletteIndex = root.GetPaletteIndex(color, 0);
    //}

    void CFOctreeColorQuantizer::OnFinish()
    {
    }

    int CFOctreeColorQuantizer::Leaves(OctreeNodeList& result)
    {
        OctreeNodeList activeNodes;
        m_pRoot->GetActiveNodes(activeNodes);
        for (OctreeNodeList::iterator iter = activeNodes.begin(); 
            iter != activeNodes.end();
            ++iter)
        {
            if ((*iter)->IsLeaf())
            {
                result.push_back(*iter);
            }
        }
        return result.size();
        //return root.ActiveNodes.Where(node => node.IsLeaf); 
    }

    void CFOctreeColorQuantizer::AddLevelNode(int level, CFOctreeNode* octreeNode)
    { 
        m_pLevels[level].push_back(octreeNode); 
    }
}

