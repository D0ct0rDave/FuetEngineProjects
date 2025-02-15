// ----------------------------------------------------------------------------
/*! \class CFRenderer
 *  \brief Class for Rendering Functionlalities
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFERendererH
#define CFERendererH
// -----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEVect2.h"
#include "types/CFEMatrix.h"
#include "types/CFEColor.h"
#include "types/CFEString.h"
#include "FEEnums.h"
// -----------------------------------------------------------------------------
class CFEFont;
class CFERendererData;
class CRenderPrim;
// -----------------------------------------------------------------------------
class CFERenderer
{
    public:
            /// Constructor of the rendering system
            CFERenderer();

            /// Constructor of the rendering system
            CFERenderer(FEHandler _hParam) { Init(_hParam); };

            /// Constructor of the rendering system
            ~CFERenderer();

			/// Initializes the rendering system
            void Init(FEHandler _hParam);

            /// Finalizes the rendering system
            void Finish();

            /// Clears the render buffer
            void BeginScene();

            /// Blits the result of the render operations to the screen
            void EndScene();

            /// Retrieves the time between 2 consecutive BeginScene calls
            FEReal rGetDeltaT();

            /// Resets the timer. Avoids great DeltaT's after long processes, like loading of data.
            void ResetTimer();

            /**-----------------------------------------------------------------------------
            Virtual screen dimensions. This allow specifying coordinates in using an
            standardized reference system, independent from the actual physical screen
            dimensions. These 2 functions should be called before a Begin/End scene block.
            -----------------------------------------------------------------------------**/
			
            /// Sets the screen virtual width.
            void SetScreenVWidth(uint _uiScrVWidth);

            /// Sets the virtual screen height.
            void SetScreenVHeight(uint _uiScrVHeight);

            /// Retrieves the virtual screen width.
            uint uiGetScreenVWidth();

            /// Retrieves the virtual screen height.
            uint uiGetScreenVHeight();

            /// Sets the current blend mode.
            void SetBlendMode(EFEBlendMode _eBlendMode);

            /// Sets the current material.
            void SetMaterial(FEHandler _hMaterial);

            /// Sets the current font.
            void SetFont(CFEFont* _poFont);
            
            /// Sets the current depth for the subsequent rendering calls.
            void SetDepth(FEReal _rDepth);
            
            /// Renders a rectangle using the current material.
            void RenderRect(FEReal _rIX,FEReal _rIY,FEReal _rFX,FEReal _rFY,
                            FEReal _rIU,FEReal _rIV,FEReal _rFU,FEReal _rFV,
                            const CFEColor& _oColor);
            
            /// Renders a quad with the given initial and final vertex and texture coordinates and color, using the current material.
            void RenderQuad(FEReal _rIX,FEReal _rIY,FEReal _rFX,FEReal _rFY,
                            FEReal _rIU,FEReal _rIV,FEReal _rFU,FEReal _rFV,
                            const CFEColor& _oColor);            
            
            /// Renders a quad using the provided vertices and texcoords, using the current material.
            void RenderQuad(CFEVect2* _poVXs,CFEVect2* _poUVs,
                            const CFEColor& _oColor);
            
            /// Renders a quad using the provided vertices, texcoords and colors, using the current material.
            void RenderQuad(CFEVect2* _poVXs,CFEVect2* _poUVs,CFEColor* _poColor);

            /// Renders a billboard using the current material.
			void RenderSprite(const CFEVect2& _oPos,const CFEVect2& _oPivot,const CFEVect2& _oScale,FEReal _rAngle,
							  const CFEVect2& _oIUV,const CFEVect2& _oFUV,
                              const CFEColor& _oColor,FEReal _rAlpha);

            /// Renders text using the current font.
            void RenderText(const CFEString& _sStr,
							FEReal _rX,FEReal _rY,
							const CFEColor& _oColor,
							EFETextHAlignmentMode _eHAlignment = THAM_DEFAULT,
							EFETextVAlignmentMode _eVAlignment = TVAM_DEFAULT,							
							FEReal _rFontPointSize = _1r);

            /// Renders a triangle mesh, using the current material.
            void RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,CFEColor* _poVCs,uint _uiNumPoints);

            /// Renders a line with the given color.
            void RenderLine(FEReal _rIX,FEReal _rIY,FEReal _rFX,FEReal _rFY,const CFEColor& _oColor);

            /// Renders a rectangle with the given initial and final coodinates and color.
            void RenderRect(FEReal _rIX,FEReal _rIY,FEReal _rFX,FEReal _rFY,const CFEColor& _oColor);

            /// Sets the current transform to the identity.
            void ResetTransform();

            /// Pushes a copy of the current transform into the stack
            void PushTransform();

            /// Pops the top matrix of the stack.
            void PopTransform();

            /// Retrieves the current transformation matrix in the top of the stack.
            const CFEMatrix& oGetTransform();

            /// Scale the current transform by the given scale.
            void ScaleTransform(FEReal _rScaleX,FEReal _rScaleY);

            /// Translates the current transform by the given parameters.
            void TranslateTransform(FEReal _rX,FEReal _rY);

            /// Rotates The transform by the given angle.
            void RotateTransform(FEReal _rAngle);

            /// Reset the camera to the default state.
            void ResetCamera();

            /// Sets the current camera position. All the primitives will be rendered as if they are viewed from the given position.
            void TranslateCamera(FEReal _rX,FEReal _rY);

            /// Retrieves the current renderer camera translation.
            CFEVect2 oGetCameraTranslation();

            /// Sets the current camera zoom.
            void ZoomCamera(FEReal _rZoom);

            /// Retrieves the current camera zoom.
            FEReal rGetCameraZoom();

            /// Retrieves a renderer property if it exists.
            bool bGetProperty(const CFEString& _sProperty,FEPointer _pParam);

            /// Sets a renderer property if it exists.
            bool bSetProperty(const CFEString& _sProperty,FEPointer _pParam);
	        
		private:

            // Internal function to render a primitive.
            void RenderPrimitive(CFEVect2* _poVX,CFEVect2* _poUV,CFEColor* _poVC,FEHandler _hMat,uint _uiPrimType,uint _uiNumVXs = 0,unsigned short* _pusIdx = NULL);

            // Pushes a primitive into the render primitive list.
            void PushRenderPrim(CFEVect2* _poVX,CFEVect2* _poUV,CFEColor* _poVC,FEHandler _hMat,uint _uiPrimType,uint _uiNumVXs = 0,unsigned short* _pusIdx = NULL);
            
            /// Fills a rendering primitive using the given data.
            void FillRenderPrim(CRenderPrim* _poRP,CFEVect2* _poVX,CFEVect2* _poUV,CFEColor* _poVC,FEHandler _hMat,uint _uiPrimType,uint _uiNumVXs,unsigned short* _pusIdx);

            // Flushes the contents of the render primitive list.
            void FlushRenderPrims();

            // Update the transformation matrix after camera or transform modifications.
            void UpdateTransform();

            // Renderer Internal Data.
            CFERendererData*    m_poData;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
