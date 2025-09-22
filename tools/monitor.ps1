param(
  [string]$ComPort,
  [int]$Baud = 115200
)

$ErrorActionPreference = 'Stop'

function Find-CdcComPort {
  $ports = Get-CimInstance Win32_SerialPort | Sort-Object -Property DeviceID
  foreach ($p in $ports) {
    if ($p.Name -match 'STMicro|USB|ST-LINK|STM32|CP210|CH340') { return $p.DeviceID }
  }
  if ($ports) { return $ports[0].DeviceID }
  return $null
}

if (-not $ComPort) { $ComPort = Find-CdcComPort }
if (-not $ComPort) { Write-Error 'No serial COM port found. Provide -ComPort COMx.' }

Write-Host "Opening $ComPort @ $Baud... (Ctrl+C to exit)"
$port = New-Object System.IO.Ports.SerialPort $ComPort,$Baud,'None',8,'One'
$port.Handshake = 'None'
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
    if ([Console]::KeyAvailable) {
      $k = [Console]::ReadKey($true)
      if ($k.Key -eq 'Enter') {
        $port.Write("`r`n")
      } elseif ($k.Key -eq 'C' -and $k.Modifiers -band [ConsoleModifiers]::Control) {
        break
      } else {
        if ($k.KeyChar -ne 0) { $port.Write([string]$k.KeyChar) }
      }
    }
    Start-Sleep -Milliseconds 5
  }
}
finally {
  if ($port.IsOpen) { $port.Close() }
  Write-Host "`nClosed $ComPort"
}