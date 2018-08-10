import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""

        self.positives = []
        with open(positives) as lines:
            for line in lines:
                if not line.startswith(';'):
                    self.positives.append(line.strip('\n'))

        self.negatives = []
        with open(negatives) as lines:
            for line in lines:
                if not line.startswith(';'):
                    self.negatives.append(line.strip('\n'))


    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        score = 0
        for token in tokens:
            if token.lower() in self.positives:
                score += 1
            elif token.lower() in self.negatives:
                score -= 1
            else:
                continue
        return score
