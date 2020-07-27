#ifdef USE_WXWIDGETS

#ifndef __BASIC_PICTURE_H__
#define __BASIC_PICTURE_H__

#include <wx/wx.h>
#include <wx/dcbuffer.h>

// ****************************************************************************
// ****************************************************************************

class BasicPicture : public wxPanel
{
  // **************************************************************************
  // Public functions.
  // **************************************************************************

public:
  BasicPicture(wxWindow *parent);
  virtual void draw(wxDC &dc);
  wxBitmap *getBitmap();

  // **************************************************************************
  // Private data.
  // **************************************************************************

private:
  /// Background bitmap of the picture for double-buffered painting
  wxBitmap *bitmap;    

  // **************************************************************************
  // Private functions.
  // **************************************************************************

private:
  void OnPaint(wxPaintEvent &event);
  void OnEraseBackground(wxEraseEvent &event);

  // ****************************************************************************
  // Declare the event table right at the end
  // ****************************************************************************

  DECLARE_EVENT_TABLE()
};

#endif 

#endif // USE_WXWIDGETS
