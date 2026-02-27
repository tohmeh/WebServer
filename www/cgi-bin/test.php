#!/usr/bin/php-cgi
<!DOCTYPE html>
<html lang="en">
<head>
    <title>PHP CGI Test</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            background-color: #f5f5f7;
            color: #1d1d1f;
            padding: 40px 20px;
            line-height: 1.6;
        }

        .container {
            max-width: 800px;
            margin: 0 auto;
            background: white;
            border-radius: 12px;
            box-shadow: 0 2px 12px rgba(0, 0, 0, 0.08);
            overflow: hidden;
        }

        .header {
            background: #667eea;
            color: white;
            padding: 32px;
            text-align: center;
        }

        .header h1 {
            font-size: 2em;
            font-weight: 600;
        }

        .content {
            padding: 32px;
        }

        .section {
            margin-bottom: 32px;
        }

        .section:last-child {
            margin-bottom: 0;
        }

        h2 {
            font-size: 1.2em;
            font-weight: 600;
            color: #667eea;
            margin-bottom: 16px;
        }

        .info-box {
            background: #f9fafb;
            border: 1px solid #e5e7eb;
            border-radius: 8px;
            padding: 16px;
            margin-bottom: 12px;
        }

        .info-box:last-child {
            margin-bottom: 0;
        }

        .label {
            font-size: 0.85em;
            font-weight: 600;
            color: #6b7280;
            text-transform: uppercase;
            letter-spacing: 0.5px;
            margin-bottom: 8px;
        }

        .value {
            font-family: 'SF Mono', Monaco, monospace;
            font-size: 0.95em;
            color: #1d1d1f;
            word-break: break-all;
        }

        .badge {
            display: inline-block;
            padding: 6px 12px;
            border-radius: 4px;
            font-size: 0.9em;
            font-weight: 600;
            color: white;
        }

        .badge-get { background: #10b981; }
        .badge-post { background: #ef4444; }
        .badge-delete { background: #f59e0b; }
        .badge-other { background: #667eea; }

        .status {
            background: #d1fae5;
            border: 1px solid #10b981;
            color: #065f46;
            padding: 16px;
            border-radius: 8px;
            text-align: center;
            font-weight: 600;
            margin-top: 24px;
        }

        .back-btn {
            display: inline-block;
            margin-top: 24px;
            padding: 10px 20px;
            background: #667eea;
            color: white;
            text-decoration: none;
            border-radius: 6px;
            font-weight: 600;
            font-size: 0.95em;
        }

        table {
            width: 100%;
            border-collapse: collapse;
        }

        td {
            padding: 8px;
            border-bottom: 1px solid #e5e7eb;
            vertical-align: top;
        }

        td:first-child {
            font-weight: 600;
            color: #6b7280;
            width: 35%;
            font-size: 0.9em;
        }

        td:last-child {
            font-family: 'SF Mono', Monaco, monospace;
            font-size: 0.85em;
            word-break: break-all;
        }

        tr:last-child td {
            border-bottom: none;
        }

        .empty {
            color: #9ca3af;
            font-style: italic;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>PHP CGI Test</h1>
        </div>

        <div class="content">
            <div class="section">
                <h2>Request Information</h2>

                <div class="info-box">
                    <div class="label">Method</div>
                    <div class="value">
                        <?php
                        $method = strtoupper($_SERVER['REQUEST_METHOD']);
                        $badgeClass = 'badge-other';
                        if ($method === 'GET') $badgeClass = 'badge-get';
                        elseif ($method === 'POST') $badgeClass = 'badge-post';
                        elseif ($method === 'DELETE') $badgeClass = 'badge-delete';
                        echo '<span class="badge ' . $badgeClass . '">' . htmlspecialchars($method) . '</span>';
                        ?>
                    </div>
                </div>

                <div class="info-box">
                    <div class="label">URI</div>
                    <div class="value"><?php echo htmlspecialchars($_SERVER['REQUEST_URI'] ?? '/'); ?></div>
                </div>

                <?php if (!empty($_SERVER['QUERY_STRING'])): ?>
                <div class="info-box">
                    <div class="label">Query String</div>
                    <div class="value"><?php echo htmlspecialchars($_SERVER['QUERY_STRING']); ?></div>
                </div>
                <?php endif; ?>

                <?php if ($method === 'POST'): ?>
                <div class="info-box">
                    <div class="label">POST Body</div>
                    <div class="value">
                        <?php
                        header('Content-Type: text/plain'); // plain text output
                        $input = file_get_contents('php://input');
                        echo "Received POST data:\n" . $input;
                        ?>
                    </div>
                </div>
                <?php endif; ?>
            </div>

            <div class="section">
                <h2>PHP Information</h2>
                <div class="info-box">
                    <table>
                        <tr>
                            <td>PHP Version</td>
                            <td><?php echo phpversion(); ?></td>
                        </tr>
                        <tr>
                            <td>SAPI</td>
                            <td><?php echo php_sapi_name(); ?></td>
                        </tr>
                        <tr>
                            <td>Script Path</td>
                            <td><?php echo htmlspecialchars($_SERVER['SCRIPT_FILENAME'] ?? 'N/A'); ?></td>
                        </tr>
                    </table>
                </div>
            </div>

            <div class="status">
        PHP CGI is working correctly
        </div>

        <!-- POST Form -->
        <div class="info-box">
            <h2>Send POST Data</h2>
            <form action="/cgi-bin/test.php" method="POST">
                <textarea id="data" name="data" rows="4" style="width:100%; padding:8px; border-radius:6px; border:1px solid #d1d5db;" placeholder="Enter your data here"></textarea><br>
                <button type="submit" style="margin-top:10px; padding:10px 20px; background:#667eea; color:white; border:none; border-radius:6px; font-weight:600; cursor:pointer;">Send POST</button>
            </form>
        </div>

            <a href="/" class="back-btn">Back to Home</a>
        </div>
    </div>
</body>
</html>
