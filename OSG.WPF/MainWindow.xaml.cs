using OSG.Cpp.CLI;
using System.Windows;
using System.Windows.Forms;

namespace OSG.WPF
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        /// <summary>
        /// The drawing area for OSG Viewer.
        /// </summary>
        private PictureBox _drawingArea;

        /// <summary>
        /// The wrapper to the managed C++/CLI
        /// </summary>
        private Wrapper wrapper;

        /// <summary>
        /// Initializes a new instance of the <see cref="MainWindow"/> class.
        /// </summary>
        public MainWindow()
        {
            InitializeComponent();

            // Initializes the wrapper and the drawing area
            wrapper = new Wrapper();
            _drawingArea = new PictureBox();

            // Construct the drawing area
            windowsFormHost.Child = _drawingArea;
            _drawingArea.Paint += new PaintEventHandler(_drawingArea_Paint);
        }

        /// <summary>
        /// Handles the Paint event of the _drawingArea control.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="PaintEventArgs"/> instance containing the event data.</param>
        private void _drawingArea_Paint(object sender, PaintEventArgs e)
        {
            // Renders the OSG Viewer into the drawing area
            wrapper.Render(_drawingArea.Handle);
        }

        /// <summary>
        /// Handles the Closed event of the Window control.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="System.EventArgs"/> instance containing the event data.</param>
        private void Window_Closed(object sender, System.EventArgs e)
        {
            // Release everything we tied
            wrapper.Destroy();
            _drawingArea.Paint -= _drawingArea_Paint;
        }
    }
}
