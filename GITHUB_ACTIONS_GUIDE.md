# GitHub Actions Guide for Jenkins Users

## Quick Mapping: Jenkins → GitHub Actions

| Jenkins | GitHub Actions |
|---------|---|
| **Pipeline** | Workflow (`.yml` file in `.github/workflows/`) |
| **Stages** | Jobs (run in parallel by default) |
| **Steps** | Steps (within a job, run sequentially) |
| **Triggers** (webhooks) | `on:` (push, pull_request, schedule, etc.) |
| **Agents** (nodes) | `runs-on:` (ubuntu-latest, windows-latest, etc.) |
| **Plugins** | Actions (GitHub Marketplace) |
| **Variables/Secrets** | Environment variables & GitHub Secrets |
| **Post-build actions** | `if: success()`, `if: failure()`, `always()` |

---

## 1. Basic Concepts

### Workflow File Structure
```yaml
name: My Pipeline Name

on:                          # TRIGGERS (like Jenkins webhooks)
  push:
    branches: [main]

jobs:                        # JOBS (like Jenkins stages)
  my-job:
    runs-on: ubuntu-latest   # RUNNER (like Jenkins agent)
    
    steps:                   # STEPS (like Jenkins sh commands)
      - uses: actions/checkout@v4
      - run: echo "Hello!"
```

### Key Differences from Jenkins:
- ✅ **Free CI/CD** - No server to maintain
- ✅ **No separate agents** - Runners are managed by GitHub
- ✅ **Jobs run in PARALLEL by default** (unlike Jenkins stages which run sequentially)
- ✅ **Use `needs:` to create dependencies** between jobs
- ✅ **Secrets managed natively** - No Jenkins credential plugins needed

---

## 2. Triggers (`on:`)

Like Jenkins webhooks, these determine WHEN your workflow runs:

```yaml
on:
  push:
    branches: [main, develop]           # Run on push to these branches
    paths: ['src/**']                    # Only if these files change
  
  pull_request:
    branches: [main]                     # Run on PR to main
  
  schedule:
    - cron: '0 2 * * 1'                 # Run every Monday at 2 AM UTC
  
  workflow_dispatch:                     # Manual trigger (like Jenkins "Build Now")
  
  release:
    types: [published]                   # Run when release is published
```

---

## 3. Jobs & Steps

### Sequential Execution (Like Jenkins Stages)

**Option A: Single job with steps (steps run sequentially)**
```yaml
jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - run: npm install      # Step 1
      - run: npm test         # Step 2 (only if Step 1 succeeds)
      - run: npm run build    # Step 3 (only if Step 2 succeeds)
```

**Option B: Multiple jobs with dependencies**
```yaml
jobs:
  lint:
    runs-on: ubuntu-latest
    steps:
      - run: npm run lint
  
  test:
    needs: lint              # Waits for lint to complete
    runs-on: ubuntu-latest
    steps:
      - run: npm test
  
  build:
    needs: test              # Waits for test to complete
    runs-on: ubuntu-latest
    steps:
      - run: npm run build
```

### Parallel Execution

Jobs run in **parallel** by default:
```yaml
jobs:
  lint:
    runs-on: ubuntu-latest
    steps: [...]   # Runs immediately
  
  test:
    runs-on: ubuntu-latest
    steps: [...]   # Runs immediately (in parallel with lint)
  
  security-scan:
    runs-on: ubuntu-latest
    steps: [...]   # Runs immediately (in parallel with lint & test)
  
  deploy:
    needs: [lint, test, security-scan]  # Waits for ALL to complete
    steps: [...]
```

---

## 4. Runners (Like Jenkins Agents)

```yaml
runs-on: ubuntu-latest           # Default Linux runner
# or
runs-on: windows-latest          # Windows runner
# or
runs-on: macos-latest            # macOS runner
# or
runs-on: self-hosted             # Your own server (like Jenkins agent)
```

---

## 5. Actions (Like Jenkins Plugins)

Instead of installing plugins, use **Actions** from GitHub Marketplace:

```yaml
steps:
  # Checkout code
  - uses: actions/checkout@v4
  
  # Setup Node.js
  - uses: actions/setup-node@v4
    with:
      node-version: '18'
  
  # Setup Python
  - uses: actions/setup-python@v4
    with:
      python-version: '3.11'
  
  # Cache dependencies (speeds up builds)
  - uses: actions/cache@v3
    with:
      path: ~/.npm
      key: ${{ runner.os }}-npm-${{ hashFiles('**/package-lock.json') }}
  
  # Upload artifacts
  - uses: actions/upload-artifact@v3
    with:
      name: build-output
      path: dist/
```

---

## 6. Environment Variables & Secrets

### Using Environment Variables
```yaml
env:
  NODE_ENV: production

jobs:
  build:
    steps:
      - run: echo $NODE_ENV  # Outputs: production
```

### Using Secrets (like Jenkins credentials)
1. Go to **Settings → Secrets and variables → Actions**
2. Click "New repository secret"
3. Name: `MY_SECRET`, Value: `your-secret-value`

Then use in workflow:
```yaml
jobs:
  deploy:
    steps:
      - name: Deploy
        run: |
          curl -X POST https://api.example.com/deploy \
            -H "Authorization: Bearer ${{ secrets.MY_SECRET }}"
```

---

## 7. Conditional Execution

Like Jenkins post-build actions:

```yaml
steps:
  - run: npm test
  
  # Run only if previous step succeeded
  - name: Build
    if: success()
    run: npm run build
  
  # Run only if previous step failed
  - name: Notify failure
    if: failure()
    run: echo "Tests failed!"
  
  # Always run (even if previous steps fail)
  - name: Cleanup
    if: always()
    run: rm -rf /tmp/*
```

---

## 8. Matrix Builds

Test across multiple versions (like Jenkins matrix builds):

```yaml
jobs:
  test:
    runs-on: ubuntu-latest
    
    strategy:
      matrix:
        node-version: [16, 18, 20]
        os: [ubuntu-latest, windows-latest]
    
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-node@v4
        with:
          node-version: ${{ matrix.node-version }}
      - run: npm test
      
# This creates 6 parallel jobs (3 node versions × 2 OS combinations)
```

---

## 9. Artifacts & Caching

### Upload Build Artifacts
```yaml
- name: Build
  run: npm run build

- name: Upload artifacts
  uses: actions/upload-artifact@v3
  with:
    name: build-dist
    path: dist/
    retention-days: 30
```

### Caching (Speeds up builds)
```yaml
- name: Cache npm dependencies
  uses: actions/cache@v3
  with:
    path: ~/.npm
    key: ${{ runner.os }}-npm-${{ hashFiles('**/package-lock.json') }}
    restore-keys: |
      ${{ runner.os }}-npm-

- run: npm install  # Skipped if cache hit
```

---

## 10. File Structure

```
your-repo/
├── .github/
│   └── workflows/
│       ├── 01-basic-workflow.yml
│       ├── 02-parallel-jobs.yml
│       └── 03-advanced-workflow.yml
├── src/
├── tests/
├── package.json
└── README.md
```

---

## 11. Viewing Results

1. Go to **Actions** tab in your repo
2. Click on a workflow run
3. Click on a job to see logs
4. View step details and errors

---

## Common Examples

### Python Project
```yaml
name: Python CI
on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    strategy:
      matrix:
        python-version: ['3.9', '3.10', '3.11']
    
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-python@v4
        with:
          python-version: ${{ matrix.python-version }}
      - run: pip install -r requirements.txt
      - run: pytest
```

### Docker Build & Push
```yaml
name: Docker Build
on: [push]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - uses: docker/setup-buildx-action@v2
      - uses: docker/build-push-action@v4
        with:
          context: .
          push: true
          tags: my-app:latest
```

---

## Resources

- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [GitHub Marketplace Actions](https://github.com/marketplace?type=actions)
- [Workflow Syntax Reference](https://docs.github.com/en/actions/using-workflows/workflow-syntax-for-github-actions)

---

## Next Steps

1. ✅ Review the example workflows in `.github/workflows/`
2. ✅ Push a commit to trigger a workflow
3. ✅ View the workflow run in the **Actions** tab
4. ✅ Modify workflows based on your project needs
