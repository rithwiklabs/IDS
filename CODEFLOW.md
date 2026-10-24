# Code Flow - SSH Brute-Force Detection IDS

## What Does This Project Do?

This project checks SSH login logs and generates an alert when the same IP address has too many failed login attempts within a short time.

## How Does It Work?

1. **Collect Logs:** `main.sh` copies `auth.log` from the Metasploitable machine to Kali Linux.
2. **Filter Failed Logins:** `grep` and `sed` find failed SSH password attempts and extract the IP address and time.
3. **Create `log.txt`:** Bash converts each time into seconds and saves the records in this format:

   `IP_ADDRESS TIME FAIL`

4. **Compile the C Program:** `gcc ids.c -o ids` creates the executable named `ids`.
5. **Run the Detector:** `./ids` reads `log.txt` and checks failed attempts from each IP address.
6. **Check the Rule:** The IDS generates an alert when an IP has **5 or more failed attempts within 60 seconds**.
7. **Save the Alert:** The alert appears in the terminal and is appended to `alert.txt`.

## Simple Flowchart

```text
Start main.sh
     |
     v
Copy auth.log from target
     |
     v
Find failed SSH logins
     |
     v
Create log.txt
     |
     v
Compile and run ids.c
     |
     v
Check attempts by IP
     |
     v
5 or more failures in 60 seconds?
     |
   Yes --> Display alert and save to alert.txt
   No  --> Display no brute-force detected
```

## How `ids.c` Works

The C program reads the processed login records from `log.txt` and checks them one by one.

- **Read Records:** Each record contains an IP address, a timestamp, and the login status.
- **Compare IP Addresses:** The program compares IP addresses to group failed attempts from the same source.
- **Count Failed Attempts:** It counts failures within the previous 60 seconds.
- **Check the Threshold:** If an IP reaches 5 or more failed attempts within 60 seconds, the program identifies a possible brute-force attack.
- **Generate an Alert:** The alert is displayed in the terminal and saved to `alert.txt`.
- **Avoid Repeated Alerts:** The `flagged` array remembers IP addresses already alerted during the current run, preventing another alert for the same IP during that execution.

### Important Constants

```c
#define limit 5
#define window 60
#define max 1000
```

- `limit` — Minimum number of failed attempts required to trigger an alert.
- `window` — Detection time window in seconds.
- `max` — Maximum number of login records stored in the array.

## Main Files

| File | Purpose |
|---|---|
| `main.sh` | Runs the complete process. |
| `auth.log` | Copy of the target's SSH authentication log. |
| `log.txt` | Processed failed-login records. |
| `ids.c` | C source code that detects brute-force patterns. |
| `ids` | Compiled executable program. |
| `alert.txt` | Stores generated alert reports. |

## Important Notes

- The IDS analyzes collected logs; it does not monitor SSH activity in real time.
- Duplicate alerts are suppressed for the same IP during one execution, but not across separate executions.
- Timestamps are stored as seconds since midnight, so records spanning midnight may not be compared correctly.
- The IDS generates alerts but does not block IP addresses or prevent login attempts.

## Summary

This project combines Bash automation and C programming to detect potential SSH brute-force attacks. Bash collects and processes the logs, while the C program analyzes failed login attempts and generates alerts when the configured threshold is reached.
