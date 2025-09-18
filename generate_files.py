import os
import random

# Directory to store generated files
output_dir = "random_text_files"
os.makedirs(output_dir, exist_ok=True)

# Expanded professional vocabulary
word_pool = [
    # Technology
    "algorithm", "data", "system", "analysis", "process", "network", "security", "encryption", "firewall", "integration",
    "design", "model", "performance", "management", "optimization", "cloud", "scalability", "efficiency", "implementation",
    "database", "structure", "computation", "automation", "strategy", "solution", "framework", "evaluation", "innovation",
    "simulation", "parallel", "storage", "learning", "statistics", "accuracy", "reliability", "distribution", "workflow",
    "prediction", "classification", "indexing", "query", "visualization", "architecture", "monitoring", "protocol",
    "transaction", "deployment", "virtualization", "bandwidth", "latency", "processing", "connectivity", "scanning",
    "iteration", "execution", "environments", "containerization", "pipeline", "repository", "versioning", "regression",
    
    # Business & management
    "leadership", "collaboration", "strategy", "decision", "forecast", "revenue", "budget", "investment", "stakeholder",
    "operation", "planning", "risk", "control", "policy", "compliance", "audit", "governance", "regulation",
    "partnership", "communication", "negotiation", "marketing", "customer", "branding", "growth", "profit",
    "sustainability", "reporting", "benchmark", "supply", "demand", "resources", "inventory", "productivity", "insight",
    "deliverable", "timeline", "objective", "result", "initiative", "culture", "coordination",
    
    # Research & academia
    "theory", "experiment", "hypothesis", "observation", "measurement", "discovery", "validation", "publication", "journal",
    "conference", "survey", "peer", "evidence", "case", "sample", "methodology", "literature", "review",
    "citation", "impact", "conclusion", "abstract", "discussion", "replication", "finding",
    
    # General professional vocabulary
    "development", "teamwork", "documentation", "presentation", "project", "task", "deadline", "milestone", "goal",
    "opportunity", "challenge", "solution", "improvement", "transformation", "training", "skills", "knowledge",
    "expertise", "quality", "standard", "procedure", "report", "feedback", "summary", "overview",
    "recommendation", "vision", "mission", "support", "responsibility", "contribution", "engagement",
    "assessment"
]

# Total files and words per file
num_files = 1000
words_per_file = 5000   # each file has 5000 words

for file_idx in range(1, num_files + 1):
    words = []
    for _ in range(words_per_file):
        # Pick only from professional word pool
        word = random.choice(word_pool)
        words.append(word)
    
    # Shuffle to avoid obvious patterns
    random.shuffle(words)
    
    # Save to file
    file_name = os.path.join(output_dir, f"file_{file_idx}.txt")
    with open(file_name, "w") as f:
        f.write(" ".join(words))

print(f"✅ Generated {num_files} files with {words_per_file} words each in '{output_dir}'")
