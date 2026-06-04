# GitHub Actions Learning - Example Workflows

This repository contains step-by-step examples of GitHub Actions workflows designed for Jenkins users transitioning to GitHub Actions.

## 📁 Workflow Files

### 1. **01-basic-workflow.yml** - Sequential Execution
- Shows how to run steps sequentially (like Jenkins pipeline stages)
- Triggers on push and pull requests
- Checkout → Install → Test → Build

**Use case:** Simple CI/CD for projects with linear build stages

### 2. **02-parallel-jobs.yml** - Parallel Execution with Dependencies
- Demonstrates multiple jobs running in parallel
- Uses `needs:` to create dependencies
- Lint, Test, Build run in parallel → Deploy runs after all succeed

**Use case:** Optimize pipeline by running independent checks simultaneously

### 3. **03-advanced-workflow.yml** - Matrix Builds & Caching
- Tests across multiple Node.js versions and operating systems
- Implements dependency caching for faster builds
- Uploads artifacts for later use
- Includes security scanning

**Use case:** Multi-platform compatibility testing, production-ready workflows

---

## 🚀 Quick Start

### Step 1: Push Code to Trigger Workflow
```bash
git add .
git commit -m "Trigger GitHub Actions"
git push origin main
```

### Step 2: View Workflow Results
- Go to **Actions** tab in your repository
- Click on the workflow run
- View job logs and step details

### Step 3: Make Changes
Edit `.github/workflows/*.yml` to customize for your project

---

## 🔄 Jenkins → GitHub Actions Comparison

| Feature | Jenkins | GitHub Actions |
|---------|---------|---|
| Configuration | `Jenkinsfile` (root) | `.github/workflows/*.yml` |
| Stages | Sequential by default | Parallel by default |
| Dependencies | `agent any` + `post` | `needs:` keyword |
| Plugins | Marketplace plugins | GitHub Actions (marketplace) |
| Secrets | Credentials plugin | Settings → Secrets |
| Runners | Jenkins agents | GitHub runners (ubuntu, windows, mac) |
| Logs | Build console | Actions tab |

---

## 📋 Common Modifications

### Change Node.js Version
```yaml
- uses: actions/setup-node@v4
  with:
    node-version: '20'  # Change this
```

### Add Environment Variables
```yaml
env:
  NODE_ENV: production
  LOG_LEVEL: debug
```

### Use GitHub Secrets
1. Go to **Settings** → **Secrets and variables** → **Actions**
2. Click **New repository secret**
3. Add name and value
4. Use in workflow: `${{ secrets.SECRET_NAME }}`

### Run Only on Specific Branches
```yaml
on:
  push:
    branches: [main, develop]  # Only these branches
```

### Trigger Manually
```yaml
on:
  workflow_dispatch:  # Adds "Run workflow" button
```

---

## 🎯 Learning Exercises

1. **Exercise 1:** Modify workflow to test Python instead of Node.js
2. **Exercise 2:** Add a step to upload coverage reports as artifacts
3. **Exercise 3:** Create a matrix build for 3 different OS versions
4. **Exercise 4:** Add conditional steps with `if: failure()`
5. **Exercise 5:** Create a scheduled workflow that runs daily

---

## 📚 Resources

- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [GitHub Actions Marketplace](https://github.com/marketplace?type=actions)
- [Workflow Syntax Reference](https://docs.github.com/en/actions/using-workflows/workflow-syntax-for-github-actions)
- [GITHUB_ACTIONS_GUIDE.md](./GITHUB_ACTIONS_GUIDE.md) - Detailed guide

---

## 💡 Pro Tips

✅ **Use caching** to speed up builds (npm, pip, maven, etc.)
✅ **Run tests in parallel** to reduce total execution time
✅ **Upload artifacts** to preserve build outputs
✅ **Use matrix builds** to test multiple versions
✅ **Add status badges** to your README
✅ **Use branch protection** to require checks before merge

---

## 🔗 Status Badge

Add this to your README to show workflow status:

```markdown
[![Workflow Status](https://github.com/YOUR_USERNAME/github-actions-learning/actions/workflows/01-basic-workflow.yml/badge.svg)](https://github.com/YOUR_USERNAME/github-actions-learning/actions)
```

---

Happy learning! 🎉
