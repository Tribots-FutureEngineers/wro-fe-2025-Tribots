

<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <title>Tribots — WRO 2025 | Team Profile</title>
  <style>
    :root{
      --accent:#0ea5a4;
      --dark:#0f172a;
      --muted:#6b7280;
      --card:#0b1220;
      --glass: rgba(255,255,255,0.03);
      font-family: Inter, ui-sans-serif, system-ui, -apple-system, 'Segoe UI', Roboto, 'Helvetica Neue', Arial;
    }
    *{box-sizing:border-box}
    body{margin:0;background:linear-gradient(180deg,#071028 0%, #041426 100%);color:#e6eef6;min-height:100vh}
    .container{max-width:1000px;margin:40px auto;padding:24px}
    header{display:flex;gap:16px;align-items:center}
    .logo{width:96px;height:96px;flex:0 0 96px;background:linear-gradient(135deg,var(--accent),#2563eb);border-radius:18px;display:flex;align-items:center;justify-content:center;box-shadow:0 6px 20px rgba(2,6,23,0.6)}
    .logo svg{width:64px;height:64px}
    h1{margin:0;font-size:28px}
    .subtitle{color:var(--muted);margin-top:6px}

    .card{background:linear-gradient(180deg, rgba(255,255,255,0.02), rgba(0,0,0,0.02));border-radius:12px;padding:18px;margin-top:18px;box-shadow:0 8px 30px rgba(2,6,23,0.6);border:1px solid rgba(255,255,255,0.03)}

    nav{display:flex;gap:8px;margin-left:auto}
    .btn{background:var(--glass);padding:8px 12px;border-radius:8px;color:var(--accent);border:1px solid rgba(14,165,164,0.12);text-decoration:none;font-weight:600}

    .grid{display:grid;grid-template-columns:1fr 320px;gap:18px;margin-top:20px}
    .team-list{list-style:none;padding:0;margin:0}
    .team-list li{padding:10px 12px;border-radius:10px;background:linear-gradient(180deg, rgba(255,255,255,0.01), rgba(0,0,0,0.01));margin-bottom:8px;display:flex;align-items:center;gap:12px}
    .avatar{width:40px;height:40px;border-radius:10px;background:linear-gradient(180deg,#fff2,#fff0);display:flex;align-items:center;justify-content:center;color:var(--dark);font-weight:700}

    table{width:100%;border-collapse:collapse;margin-top:8px}
    th,td{padding:10px;border-bottom:1px dashed rgba(255,255,255,0.03);text-align:left}
    th{color:var(--muted);font-weight:600}

    .footer{color:var(--muted);font-size:13px;margin-top:18px}

    /* responsive */
    @media (max-width:900px){.grid{grid-template-columns:1fr;}.logo{width:76px;height:76px}.container{padding:16px}}
  </style>
</head>
<body>
  <div class="container">
    <header>
      <div class="logo" aria-hidden="true">
        <!-- Sample logo: stylized T with gear -->
        <svg viewBox="0 0 64 64" fill="none" xmlns="http://www.w3.org/2000/svg" role="img" aria-label="Tribots logo">
          <defs>
            <linearGradient id="g1" x1="0" x2="1">
              <stop offset="0" stop-color="#06b6d4" />
              <stop offset="1" stop-color="#2563eb" />
            </linearGradient>
          </defs>
          <rect x="0" y="0" width="64" height="64" rx="12" fill="url(#g1)" opacity="0.14"/>
          <g transform="translate(8,8) scale(0.875)" fill="#fff">
            <path d="M8 6h24v4H8z" opacity="0.95"/>
            <path d="M18 10v28h4V10z" opacity="0.95"/>
          </g>
          <g transform="translate(36,14)">
            <circle cx="8" cy="8" r="8" fill="#fff" opacity="0.08"/>
            <path d="M8 4 L10 8 L8 12 L6 8 Z" fill="#fff" opacity="0.9"/>
          </g>
        </svg>
      </div>

      <div>
        <h1>Tribots — WRO 2025</h1>
        <div class="subtitle">Team: <strong>Tribots - Future Engineers</strong> · Theme: <em>Professional WRO Presentation</em></div>
      </div>

      <nav>
        <a class="btn" href="#components">Components</a>
        <a class="btn" href="#team">Team</a>
        <a class="btn" href="#about">About</a>
      </nav>
    </header>

    <section class="card">
      <h2 id="about">About this project</h2>
      <p style="color:var(--muted)">This is a professional single-file HTML/CSS landing page suitable for a GitHub repository README or GitHub Pages site for the <strong>Tribots</strong> team competing in WRO 2025. It includes a sample logo (SVG), team member list, project theme, and a detailed components table you can expand.</p>

      <div class="grid">
        <div>
          <h3>Project Summary</h3>
          <p class="muted">Our team builds a line-following / challenges robot using microcontrollers, motor drivers, sensors, and an efficient power system. Below is a components list and short notes you can copy into your repo.</p>

          <h3 id="components">Bill of Materials (sample)</h3>
          <table>
            <thead>
              <tr><th>Component</th><th>Qty</th><th>Purpose</th><th>Notes</th></tr>
            </thead>
            <tbody>
              <tr><td>Arduino Nano (or Mega)</td><td>1</td><td>Robot brain / motor control / sensors</td><td>Choose Nano for compact, Mega for many IOs</td></tr>
              <tr><td>L298N Motor Driver</td><td>1</td><td>Drive DC motors (H-bridge)</td><td>Popular, but bulky and inefficient; consider TB6612 or DRV8833</td></tr>
              <tr><td>TB6612FNG</td><td>1</td><td>Motor driver (recommended)</td><td>Lower heat than L298</td></tr>
              <tr><td>Wheel encoders</td><td>2</td><td>Odometry / speed feedback</td><td>Optical or magnetic</td></tr>
              <tr><td>TOF (Time-of-Flight) sensor</td><td>1–4</td><td>Distance & obstacle detection</td><td>VL53L0X / VL53L1X common models</td></tr>
              <tr><td>Servo (high torque)</td><td>1–2</td><td>Actuator for arm or mechanism</td><td>Power carefully (separate supply recommended)</td></tr>
              <tr><td>Raspberry Pi (optional)</td><td>1</td><td>Camera processing / vision</td><td>Use Pi Camera or USB camera</td></tr>
              <tr><td>DC Motors (geared)</td><td>2–4</td><td>Locomotion</td><td>Match voltage & RPM to gearbox</td></tr>
              <tr><td>Li-ion battery pack / LiPo</td><td>1</td><td>Power source</td><td>Use proper BMS and regulator</td></tr>
              <tr><td>Buck converter (LM2596)</td><td>1</td><td>Step down battery voltage to 5V</td><td>Check cooling under load</td></tr>
              <tr><td>Voltage regulator (5V)</td><td>1</td><td>Power microcontrollers & sensors</td><td>Consider separate motor and logic supplies</td></tr>
              <tr><td>Battery connector / switch</td><td>1</td><td>Power management</td><td>Fuse recommended</td></tr>
              <tr><td>Chassis & wheels</td><td>1 set</td><td>Mechanical base</td><td>3D print or laser-cut</td></tr>
              <tr><td>Wires, headers, PCB</td><td>various</td><td>Interconnect & mounting</td><td>Use quality connectors</td></tr>
            </tbody>
          </table>

          <h3>Quick usage notes</h3>
          <ul style="color:var(--muted)">
            <li>Keep motors and servos on a separate power rail to avoid brownouts on your microcontroller.</li>
            <li>Always add decoupling capacitors and ground everything to a common ground.</li>
            <li>Test the buck converter under expected current draw — some cheap modules overheat or spike.</li>
            <li>Use `git` branches for features (e.g., <code>feat/line-follow</code>, <code>chore/hardware-docs</code>).</li>
          </ul>
        </div>

        <aside>
          <div class="card" style="padding:12px">
            <h4 id="team">Team Members</h4>
            <ul class="team-list">
              <li><div class="avatar">E</div> <div><strong>Ehtasham</strong><div style="color:var(--muted);font-size:13px">Lead - Electronics & Firmware</div></div></li>
              <li><div class="avatar">B</div> <div><strong>Behzad</strong><div style="color:var(--muted);font-size:13px">Mechanical & 3D Design</div></div></li>
              <li><div class="avatar">S</div> <div><strong>Shahim</strong><div style="color:var(--muted);font-size:13px">Software & Vision</div></div></li>
            </ul>

            <h4 style="margin-top:12px">Repo / Team</h4>
            <p style="color:var(--muted);font-size:14px;margin:0">Organization: <strong>Tribots-FutureEngineers</strong><br>Repo: <strong>wro-fe-2025-Tribots</strong></p>

            <div style="margin-top:12px">
              <a class="btn" href="#">Download BOM (CSV)</a>
            </div>
          </div>
        </aside>
      </div>

      <div class="footer">Pro tip: Put this file as <code>index.html</code> in your repo and enable GitHub Pages (Settings → Pages) to show it as a project site.</div>
    </section>
  </div>
</body>
</html>
