param(
  [string]$ComPort,
  [int]$Baud = 115200
)

$ErrorActionPreference = 'Stop'

function Find-CdcComPort {
  $ports = Get-CimInstance Win32_SerialPort | Sort-Object -Property DeviceID
  # Prefer Windows CDC class driver names
  foreach ($p in $ports) { if ($p.Name -match 'USB Serial Device|CDC|ACM') { return $p.DeviceID } }
  # Then other common USB-serial bridges
  foreach ($p in $ports) { if ($p.Name -match 'CP210|CH340|FTDI') { return $p.DeviceID } }
  # Avoid ST-LINK VCP unless explicitly chosen
  foreach ($p in $ports) { if ($p.Name -match 'ST[- ]?LINK') { return $p.DeviceID } }
  if ($ports) { return $ports[0].DeviceID }
  return $null
}

if (-not $ComPort) { $ComPort = Find-CdcComPort }
if (-not $ComPort) { Write-Error 'No serial COM port found. Provide -ComPort COMx.' }

Write-Host "Opening $ComPort @ $Baud... (Ctrl+C to exit)"
$port = New-Object System.IO.Ports.SerialPort $ComPort,$Baud,'None',8,'One'
$port.Handshake = 'None'
$port.DtrEnable = $true
$port.RtsEnable = $true
$port.NewLine = "`r`n"
$port.ReadTimeout = 50
$port.WriteTimeout = 500
$port.Open()

try {
  while ($true) {
    if ($port.BytesToRead -gt 0) {
      $data = $port.ReadExisting()
      if ($null -ne $data) { Write-Host -NoNewline $data }
    }
    Start-Sleep -Milliseconds 10
  }
}
finally {
  if ($port.IsOpen) { $port.Close() }
  Write-Host "`nClosed $ComPort"
}