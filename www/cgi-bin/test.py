#!/usr/bin/env python3
print("Content-Type: text/html\n")

import os
import sys
import html

method = os.environ.get('REQUEST_METHOD', '')
uri = os.environ.get('REQUEST_URI', '/')
query = os.environ.get('QUERY_STRING', '')
script_filename = os.environ.get('SCRIPT_FILENAME', '')
gateway = os.environ.get('GATEWAY_INTERFACE', '')
server_protocol = os.environ.get('SERVER_PROTOCOL', '')

# Read POST body if applicable
body = ''
if method == 'POST':
    length = int(os.environ.get('CONTENT_LENGTH', 0))
    body = sys.stdin.read(length) if length > 0 else ''

badge_class = 'badge-other'
if method == 'GET':
    badge_class = 'badge-get'
elif method == 'POST':
    badge_class = 'badge-post'
elif method == 'DELETE':
    badge_class = 'badge-delete'

print(f"""<!DOCTYPE html>
<html lang="en">
<head>
    <title>Python CGI Test</title>
    <style>
        * {{ margin: 0; padding: 0; box-sizing: border-box; }}
        body {{
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            background-color: #f5f5f7;
            color: #1d1d1f;
            padding: 40px 20px;
            line-height: 1.6;
        }}
        .container {{
            max-width: 800px;
            margin: 0 auto;
            background: white;
            border-radius: 12px;
            box-shadow: 0 2px 12px rgba(0, 0, 0, 0.08);
            overflow: hidden;
        }}
        .header {{
            background: #667eea;
            color: white;
            padding: 32px;
            text-align: center;
        }}
        .header h1 {{
            font-size: 2em;
            font-weight: 600;
        }}
        .content {{ padding: 32px; }}
        .section {{ margin-bottom: 32px; }}
        .section:last-child {{ margin-bottom: 0; }}
        h2 {{
            font-size: 1.2em;
            font-weight: 600;
            color: #667eea;
            margin-bottom: 16px;
        }}
        .info-box {{
            background: #f9fafb;
            border: 1px solid #e5e7eb;
            border-radius: 8px;
            padding: 16px;
            margin-bottom: 12px;
        }}
        .label {{
            font-size: 0.85em;
            font-weight: 600;
            color: #6b7280;
            text-transform: uppercase;
            letter-spacing: 0.5px;
            margin-bottom: 8px;
        }}
        .value {{
            font-family: 'SF Mono', Monaco, monospace;
            font-size: 0.95em;
            color: #1d1d1f;
            word-break: break-all;
        }}
        .badge {{
            display: inline-block;
            padding: 6px 12px;
            border-radius: 4px;
            font-size: 0.9em;
            font-weight: 600;
            color: white;
        }}
        .badge-get {{ background: #10b981; }}
        .badge-post {{ background: #ef4444; }}
        .badge-delete {{ background: #f59e0b; }}
        .badge-other {{ background: #667eea; }}
        .status {{
            background: #d1fae5;
            border: 1px solid #10b981;
            color: #065f46;
            padding: 16px;
            border-radius: 8px;
            text-align: center;
            font-weight: 600;
            margin-top: 24px;
        }}
        .back-btn {{
            display: inline-block;
            margin-top: 24px;
            padding: 10px 20px;
            background: #667eea;
            color: white;
            text-decoration: none;
            border-radius: 6px;
            font-weight: 600;
            font-size: 0.95em;
        }}
        table {{
            width: 100%;
            border-collapse: collapse;
        }}
        td {{
            padding: 8px;
            border-bottom: 1px solid #e5e7eb;
            vertical-align: top;
        }}
        td:first-child {{
            font-weight: 600;
            color: #6b7280;
            width: 35%;
            font-size: 0.9em;
        }}
        td:last-child {{
            font-family: 'SF Mono', Monaco, monospace;
            font-size: 0.85em;
            word-break: break-all;
        }}
        tr:last-child td {{ border-bottom: none; }}
        .empty {{ color: #9ca3af; font-style: italic; }}
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>Python CGI Test</h1>
        </div>

        <div class="content">
            <div class="section">
                <h2>Request Information</h2>

                <div class="info-box">
                    <div class="label">Method</div>
                    <div class="value"><span class="badge {badge_class}">{html.escape(method)}</span></div>
                </div>

                <div class="info-box">
                    <div class="label">URI</div>
                    <div class="value">{html.escape(uri)}</div>
                </div>""")

if query:
    print(f"""
                <div class="info-box">
                    <div class="label">Query String</div>
                    <div class="value">{html.escape(query)}</div>
                </div>""")

if method == 'POST':
    print(f"""
                <div class="info-box">
                    <div class="label">POST Body</div>
                    <div class="value">{html.escape(body) if body else '<span class="empty">No data</span>'}</div>
                </div>""")

print(f"""
            </div>

            <div class="section">
                <h2>Python CGI Environment</h2>
                <div class="info-box">
                    <table>
                        <tr><td>Python Version</td><td>{html.escape(sys.version.split()[0])}</td></tr>
                        <tr><td>Script Path</td><td>{html.escape(script_filename or 'N/A')}</td></tr>
                        <tr><td>Gateway Interface</td><td>{html.escape(gateway or 'N/A')}</td></tr>
                    </table>
                </div>
            </div>

            <div class="status">
                Python CGI is working correctly
            </div>

            <a href="/" class="back-btn">Back to Home</a>
        </div>
    </div>
</body>
</html>
""")
