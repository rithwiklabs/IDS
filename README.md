# SSH Brute-Force Detection IDS 

A lightweight Intrusion Detection System built using **C and Bash** to detect possible SSH brute-force attacks by analyzing authentication logs.

The system collects logs from Metasploitable 2, checks failed SSH login attempts, and generates alerts when suspicious activity is detected.

## Features

- Collects SSH logs using SCP.
- Extracts failed login attempts using `grep` and `sed`.
- Detects repeated failures from the same IP address.
- Generates alerts for 5 or more failed attempts within 60 seconds.
- Displays alerts in the terminal and saves them in `alert.txt`.
- Automates the complete process using Bash.

## Technologies Used

| Technology | Purpose |
|---|---|
| C | Detection engine |
| Bash | Workflow automation |
| GCC | C compiler |
| grep and sed | Log filtering and parsing |
| SCP and SSH | Remote log collection |
| Kali Linux | IDS host |
| Metasploitable 2 | Test target |
| VirtualBox | Virtual lab environment |

## Project Structure

```text id="8ldxog"
IDS/
├── main.sh       # Main automation script
├── ids.c         # Brute-force detection engine
├── auth.log      # Retrieved authentication log
├── log.txt       # Processed failed-login records
├── ids           # Compiled executable
└── alert.txt     # Persistent alert report
```

## How It Works

1. `main.sh` collects `auth.log` from Metasploitable 2.
2. Failed SSH login attempts are extracted and saved in `log.txt`.
3. `ids.c` checks failed attempts from each IP address.
4. If an IP has at least 5 failed attempts within 60 seconds, an alert is generated.
5. The alert is displayed in the terminal and saved in `alert.txt`.

## Setup and Execution

### Prerequisites

Ensure the Kali Linux VM has:

- GCC
- Bash
- OpenSSH client and SCP
- `grep` and `sed`

The target VM must be reachable over the lab network, and the configured account must have permission to retrieve the authentication log.

### 1. Clone the repository

```bash
git clone https://github.com/rithwiklabs/IDS.git
cd IDS
```

### 2. Configure the target

Open `main.sh` and set the target IP address and username:

```bash
tg_ip="IP-OF-TARGET-MACHINE"
tg_us="USERNAME"
```

Replace the example values with the IP address and username configured for your isolated lab.

### 3. Make the script executable

```bash
chmod +x main.sh
```

### 4. Run the IDS

```bash
./main.sh
```

The script performs log collection, processing, compilation, and detection in sequence.

## Documentation

See [`CODE_FLOW.md`](CODE_FLOW.md) for a simple explanation of the code and execution flow.

## Limitations

- Analyzes saved logs instead of monitoring SSH activity in real time.
- Does not block IP addresses.
- Duplicate alerts can occur across separate executions.
- Timestamps spanning midnight may not be handled correctly.

## Project Goal

This project demonstrates basic intrusion detection, Linux log analysis, Bash scripting, and C programming.

**Built for learning and defensive security.** 🔐

