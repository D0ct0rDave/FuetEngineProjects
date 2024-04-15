// ----------------------------------------------------------------------------
/*! \class CFRenderer
 *  \brief Class for Rendering Functionlalities
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
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

            /// Sets the current shader.
            uint uiRegisterShader(const CFEString& _sShader);

            /// Sets the current shader.
            void SetShader(uint _uiShaderID);

            /// Retrieves the current shader.
            uint uiGetShader();

            /// Sets the current blend mode.
            void SetBlendMode(EFEBlendMode _eBlendMode);

            /// Sets the current material.
            void SetMaterial(FEHandler _hMaterial);

            /// Sets the current font.
            void SetFont(CFEFont* _poFont);

            /// Sets the current depth for the subsequent rendering calls.
            void SetDepth(FEReal _rDepth);

			/// Sets the point size for the rendered text.
			void SetTextPointSize(FEReal _rTextPointSize);

			/// Sets the tracking for the rendered text.
			void SetTextTracking(FEReal _rTextTracking);

			/// Sets the interlining space for the rendered text.
			void SetTextInterlining(FEReal _rTextInterlining);

            /// Renders a line with the given color.
            void RenderLine(FEReal _rIX,FEReal _rIY,FEReal _rFX,FEReal _rFY,const CFEColor& _oColor);

            /// Renders a rectangle with the given initial and final coodinates and color without material.
            void RenderRect(FEReal _rIX,FEReal _rIY,FEReal _rFX,FEReal _rFY,const CFEColor& _oColor);

            /// Renders a quad with the given initial and final vertex and fills it with the current material and color.
            void RenderQuad(FEReal _rIX,FEReal _rIY,FEReal _rFX,FEReal _rFY,const CFEColor& _oColor);

			/// Renders a quad with the given initial and final vertex and texture coordinates and color, using the current material.
            void RenderQuadList(CFEVect2* _poVX,CFEVect2* _poUV,CFEColor* _poVC,uint _uiNumQuads);

            /// Renders a billboard using the current material.
			void RenderSprite(const CFEVect2& _oPos,const CFEVect2& _oPivot,const CFEVect2& _oScale,FEReal _rAngle,
							  const CFEVect2& _oIUV,const CFEVect2& _oFUV,
                              const CFEColor& _oColor,FEReal _rAlpha);

            /// Renders text using the current font.
            void RenderText(const CFEString& _sStr,
							FEReal _rX,FEReal _rY,
							const CFEColor& _oColor,
							EFETextHAlignmentMode _eHAlignment = THAM_DEFAULT,
							EFETextVAlignmentMode _eVAlignment = TVAM_DEFAULT);

			/// Renders a triangle mesh, using the current material.
            void RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,const CFEColor& _oColor,uint _uiNumPoints);

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

            /// Sets the current camera rotation.
            void RotateCamera(FEReal _rAngle);

            /// Retrieves the current camera rotation.
            FEReal rGetCameraRotation();

            /// Sets the current view position.
            void TranslateView(FEReal _rX,FEReal _rY);

            /// Retrieves the current renderer camera translation.
            CFEVect2 oGetViewTranslation();

            /// Sets the current camera rotation.
            void RotateView(FEReal _rAngle);

            /// Retrieves the current camera rotation.
            FEReal rGetViewRotation();

            /// Get Camera/View transform
            const CFEMatrix& oGetCamViewTransform();

            /// Retrieves a renderer property if it exists.
            FEBool bGetProperty(const CFEString& _sProperty,FEPointer _pParam);

            /// Sets a renderer property if it exists.
            FEBool bSetProperty(const CFEString& _sProperty,FEPointer _pParam);
	        
		private:

            // Update the transformation matrix after camera or transform modifications.
            void UpdateViewTransform();

            // Update the transformation matrix after camera or transform or modifications.
            void UploadTransform();

            // Renderer Internal Data.
            CFERendererData*    m_poData;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
