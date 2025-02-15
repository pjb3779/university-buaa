pipeline_list_1 = [
    # --------------------------- TODO ---------------------------------------
    ('cv',TfidfVectorizer(token_pattern=r"(?u)\b\w+\b", stop_words=stopwords) ),
    ('classifier', MultinomialNB()),
    # ------------------------------------------------------------------------
]
pipeline_list_2 = [
    # --------------------------- TODO ---------------------------------------
    ('cv',TfidfVectorizer(token_pattern=r"(?u)\b\w+\b", stop_words=stopwords) ),
    ('max_abs_scaler',preprocessing.MaxAbsScaler()),
    ('classifier', MultinomialNB()),
    # ------------------------------------------------------------------------
]
pipeline_list_3_1 = [
    # --------------------------- TODO ---------------------------------------
    ('cv',TfidfVectorizer(token_pattern=r"(?u)\b\w+\b", stop_words=stopwords)),
    ('max_abs_scaler', MaxAbsScaler()),
    ('classifier', LogisticRegression(max_iter=10)),
    # ------------------------------------------------------------------------
]
pipeline_list_3_2 = [
    # --------------------------- TODO ---------------------------------------
   ('cv',TfidfVectorizer(token_pattern=r"(?u)\b\w+\b", stop_words=stopwords)),
    ('max_abs_scaler', MaxAbsScaler()),
    ('classifier', LogisticRegression(max_iter=20)),
    # ------------------------------------------------------------------------
]
pipeline_list_4 = [
    # --------------------------- TODO ---------------------------------------
    ('cv',TfidfVectorizer(token_pattern=r"(?u)\b\w+\b", stop_words=stopwords)),
    ('max_abs_scaler', MaxAbsScaler()),
    ('classifier', LogisticRegression(max_iter=10)),
    # ------------------------------------------------------------------------
]
