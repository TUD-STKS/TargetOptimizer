#ifdef USE_WXWIDGETS

#include "BasicPicture.h"


// ****************************************************************************
// The event table.
// ****************************************************************************

BEGIN_EVENT_TABLE(BasicPicture, wxPanel)
    EVT_PAINT (BasicPicture::OnPaint)
    EVT_ERASE_BACKGROUND (BasicPicture::OnEraseBackground)
END_EVENT_TABLE()


// ****************************************************************************
// ****************************************************************************

BasicPicture::BasicPicture(wxWindow *parent) : wxPanel(parent, wxID_ANY, 
    wxDefaultPosition, wxDefaultSize,
    wxFULL_REPAINT_ON_RESIZE     // Force a complete redraw always when the window is resized
    )
{
  // Leave all background painting to the application
  SetBackgroundStyle(wxBG_STYLE_CUSTOM);

  // Initialize the background image with a small default size.
  bitmap = new wxBitmap(200, 200);
}


// ****************************************************************************
/// Reimplement this virtual function in a derived class to do all the painting.
/// This function is automatically called in the paint event handler.
/// As long as this function is not reimplemented, it will draw a test image.
// ****************************************************************************

void BasicPicture::draw(wxDC &dc)
{
  // Clear the background
  dc.SetBackground(*wxRED_BRUSH);
  dc.Clear();

  int w, h;
  this->GetSize(&w, &h);

  dc.SetPen(*wxBLACK_PEN);
  dc.DrawLine(0, 0, w-1, h-1);

  dc.SetPen(*wxWHITE_PEN);
  dc.DrawLine(w-1, 0, 0, h-1);
}


// ****************************************************************************
/// Returns the bitmap where you can paint on. See the function drawTestImage()
/// for an example.
// ****************************************************************************

wxBitmap *BasicPicture::getBitmap()
{
  // Make sure that we habe a background bitmap that is at least always
  // as big as the client area of this window.

  int w, h;
  this->GetSize(&w, &h);
  if ((bitmap->GetWidth() < w) || (bitmap->GetHeight() < h))
  {
    delete bitmap;
    bitmap = new wxBitmap(w, h);
  }

  // Return the pointer to the bitmap
  return bitmap;
}


// ****************************************************************************
// ****************************************************************************

void BasicPicture::OnPaint(wxPaintEvent &event)
{
  // ALWAYS create the DC object, wheather it is used or not.
  // On destruction of the DC object, the content of bitmap buffer is
  // automatically copied into the client area of this window.
  
  wxBufferedPaintDC dc(this, *getBitmap());

  // Draw on the device context.
  draw(dc);
}


// ****************************************************************************
/// Intercept this event to avoid flickering.
// ****************************************************************************

void BasicPicture::OnEraseBackground(wxEraseEvent &event)
{
  // Do nothing here - especially, DO NOT class event.Skip() !!
}

// ****************************************************************************

#endif // USE_WXWIDGETS