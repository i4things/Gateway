/* Copyright (c) B2N Ltd. All Rights Reserved.
 *
 * This SOURCE CODE FILE, which has been provided by B2N Ltd. as part of an B2N
 * Ltd. product for use ONLY by licensed users of the product, includes
 * Ltd. product for use ONLY by licensed users of the product, includes
 * CONFIDENTIAL and PROPRIETARY information of B2N Ltd.
 *
 * USE OF THIS SOFTWARE IS GOVERNED BY THE TERMS AND CONDITIONS OF THE LICENSE
 * STATEMENT AND LIMITED WARRANTY FURNISHED WITH THE PRODUCT.
 *
 * IN PARTICULAR, YOU WILL INDEMNIFY AND HOLD B2N LTD., ITS RELATED COMPANIES
 * AND ITS SUPPLIERS, HARMLESS FROM AND AGAINST ANY CLAIMS OR LIABILITIES
 * ARISING OUT OF THE USE, REPRODUCTION, OR DISTRIBUTION OF YOUR PROGRAMS,
 * INCLUDING ANY CLAIMS OR LIABILITIES ARISING OUT OF OR RESULTING FROM THE USE,
 * MODIFICATION, OR DISTRIBUTION OF PROGRAMS OR FILES CREATED FROM, BASED ON,
 * AND/OR DERIVED FROM THIS SOURCE CODE FILE.
 */

using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace GatewayConfig
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm());
        }
    }
}
