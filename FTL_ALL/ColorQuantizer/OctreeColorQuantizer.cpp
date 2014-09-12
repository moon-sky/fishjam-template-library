#include "stdafx.h"
#include "OctreeColorQuantizer.h"
#include <ftlGdi.h>
#include <ftlFunctional.h>

namespace FTL
{
    BYTE CFOctreeNode::s_MASK[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
    CFOctreeNode::CFOctreeNode(int level, CFOctreeColorQuantizer* pParent)
    {
        Red = 0;
        Green = 0;
        Blue = 0;
        pixelCount = 0;
        paletteIndex = 0;
        ZeroMemory(nodes, sizeof(CFOctreeNode*) * _countof(nodes));

        if (level < 7)
        {
            pParent->AddLevelNode(level, this);
        }
    }
    
    COLORREF CFOctreeNode::GetColor() const
    {
        COLORREF result = RGB(0, 0, 0);

        // determines a color of the leaf
        if (IsLeaf())
        {
            result = (pixelCount == 1 ? 
                MAKE_RGBA(Red, Green, Blue, 0xFF) : 
                MAKE_RGBA(Red/pixelCount, Green/pixelCount, Blue/pixelCount, 0xFF));
        }
        else
        {
            FTLASSERT(FALSE && TEXT("Cannot retrieve a color for other node than leaf."));
        }

        return result;
    }

    int CFOctreeNode::GetActiveNodesPixelCount() const
    {
        int result = pixelCount;

        // sums up all the pixel presence for all the active nodes
        for (int index = 0; index < 8; index++)
        {
            CFOctreeNode* node = nodes[index];

            if (node != NULL)
            {
                result += node->pixelCount;
            }
        }

        return result;
    }

    OctreeNodeList CFOctreeNode::GetActiveNodes() const
    {
        OctreeNodeList result;

        // adds all the active sub-nodes to a list
        for (int index = 0; index < 8; index++)
        {
            CFOctreeNode* node = nodes[index];

            if (node != NULL)
            {
                if (node->IsLeaf())
                {
                    result.push_back(node);
                    //result.Add(node);
                }
                else
                {
                    const OctreeNodeList& subNodes = node->GetActiveNodes();
                    std::copy(subNodes.begin(), subNodes.end(), back_inserter(result));
                    //result.AddRange(node->GetActiveNodes());
                }
            }
        }

        return result;
    }

    void CFOctreeNode::AddColor(COLORREF color, int level, CFOctreeColorQuantizer* pParent)
    {
        // if this node is a leaf, then increase a color amount, and pixel presence
        if (level == 8)
        {
            Red += GetRValue(color);
            Green += GetGValue(color);
            Blue += GetBValue(color);
            pixelCount++;
        }
        else if (level < 8) // otherwise goes one level deeper
        {
            // calculates an index for the next sub-branch
            int index = GetColorIndexAtLevel(color, level);

            // if that branch doesn't exist, grows it
            if (nodes[index] == NULL)
            {
                nodes[index] = new CFOctreeNode(level, pParent);
            }

            // adds a color to that branch
            nodes[index]->AddColor(color, level + 1, pParent);
        }
    }

    int CFOctreeNode::GetPaletteIndex(COLORREF color, int level)
    {
        int result = 0;

        // if a node is leaf, then we've found are best match already
        if (IsLeaf())
        {
            result = paletteIndex;
        }
        else // otherwise continue in to the lower depths
        {
#if 1
            int index = GetColorIndexAtLevel(color, level);
            CFOctreeNode* pNode = nodes[index];
            if (pNode != NULL)
            {
                result = pNode->GetPaletteIndex(color, level + 1);
            }
            else
            {
                for (int i = 0; i < 8; i++)
                {
                    if (nodes[i] != NULL)
                    {
                        result = nodes[i]->GetPaletteIndex(color, level + 1);
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
        for (int index = 0; index < 8; index++)
        {
            CFOctreeNode* node = nodes[index];

            if (node != NULL)
            {
                // sums up their color components
                Red += node->Red;
                Green += node->Green;
                Blue += node->Blue;

                // and pixel presence
                pixelCount += node->pixelCount;

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
        paletteIndex = index;
    }
    /*************************************************************************************************
    * 
    *************************************************************************************************/

    CFOctreeColorQuantizer::CFOctreeColorQuantizer()
    {
        root = NULL;
        lastColorCount = 0;
    }
    CFOctreeColorQuantizer::~CFOctreeColorQuantizer()
    {
        SAFE_DELETE(root);
    }

    //void CFOctreeColorQuantizer::OnAddColor(Color color, Int32 key, Int32 x, Int32 y){
    //    root->AddColor(Color, 0, this);
    //}
    
    BOOL CFOctreeColorQuantizer::OnPrepare()
    {
        BOOL bRet = __super::OnPrepare();

        levels = new OctreeNodeList[7];

        // creates the octree level lists
        //for (int level = 0; level < 7; level++)
        //{
        //    levels[level] = new List<OctreeNode>();
        //}

        // creates a root node
        root = new CFOctreeNode(0, this);

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
                root->AddColor(color, 0, this);
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
        std::list<COLORREF> result;
        OctreeNodeList curLeaves = Leaves();
        int leafCount = curLeaves.size();
        lastColorCount = leafCount;
        int paletteIndex = 0;

        // goes thru all the levels starting at the deepest, and goes upto a root level
        for (int level = 6; level >= 0; level--)
        {
            // if level contains any node
            if (levels[level].size() > 0)
            {
                // orders the level node list by pixel presence (those with least pixels are at the top)
                OctreeNodeList sortedNodeList = levels[level];
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
                levels[level].clear();
            }
        }

        // goes through all the leaves that are left in the tree (there should now be less or equal than requested)
        OctreeNodeList sortLeaves = Leaves();
        sortLeaves.sort(FTL::UnreferenceLess<CFOctreeNode*>());
        for(OctreeNodeList::reverse_iterator iter = sortLeaves.rbegin(); 
            iter != sortLeaves.rend();
            iter++)
        //foreach (OctreeNode node in Leaves.OrderByDescending(node => node.ActiveNodesPixelCount))
        {
            if (paletteIndex >= colorCount) break;

            // adds the leaf color to a palette
            if ((*iter)->IsLeaf())
            {
                result.push_back((*iter)->GetColor());
            }

            // and marks the node with a palette index
            (*iter)->SetPaletteIndex(paletteIndex++);
        }

        // we're unable to reduce the Octree with enough precision, and the leaf count is zero
        if (result.size() == 0)
        {
            FTLASSERT(FALSE && "The Octree contains after the reduction 0 colors, it may happen for 1-16 colors because it reduces by 1-8 nodes at time. Should be used on 8 or above to ensure the correct functioning.");
        }

        // returns the palette
        //return result;
        return TRUE;
    }

    //protected override void OnGetPaletteIndex(Color color, Int32 key, Int32 x, Int32 y, out Int32 paletteIndex)
    //{
    //    // retrieves a palette index
    //    paletteIndex = root.GetPaletteIndex(color, 0);
    //}

    void CFOctreeColorQuantizer::OnFinish()
    {
    }

    OctreeNodeList CFOctreeColorQuantizer::Leaves()
    {
        OctreeNodeList result;
        OctreeNodeList& activeNodes = root->GetActiveNodes();
        for (OctreeNodeList::iterator iter = activeNodes.begin(); 
            iter != activeNodes.end();
            ++iter)
        {
            if ((*iter)->IsLeaf())
            {
                result.push_back(*iter);
            }
        }
        return result;
        //return root.ActiveNodes.Where(node => node.IsLeaf); 
    }

    void CFOctreeColorQuantizer::AddLevelNode(int level, CFOctreeNode* octreeNode)
    { 
        levels[level].push_back(octreeNode); 
    }
}

