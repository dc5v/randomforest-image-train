#ifndef RANDOMFOREST_HPP
#define RANDOMFOREST_HPP

#include "parameters.hpp"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using namespace std;

struct TreeNode
{
    int feature;
    double threshold;
    double value;
    TreeNode *left;
    TreeNode *right;
};

struct RandomForest
{
    vector<TreeNode *> trees;
    int num_trees;
};

TreeNode *forest_create_node()
{
    TreeNode *node = new TreeNode();
    node->feature = -1;
    node->threshold = 0.0;
    node->value = 0.0;
    node->left = nullptr;
    node->right = nullptr;

    return node;
}

void tree_free(TreeNode *node)
{
    if (node != nullptr)
    {
        tree_free(node->left);
        tree_free(node->right);

        delete node;
    }
}

double foprest_mean(const vector<double> &values)
{
    double sum = 0.0;

    for (double value : values)
    {
        sum += value;
    }

    return sum / values.size();
}

double forest_variance(const vector<double> &values)
{
    double mean = foprest_mean(values);
    double variance = 0.0;

    for (double value : values)
    {
        variance += (value - mean) * (value - mean);
    }

    return variance / values.size();
}

double forest_mse(const vector<double> &left, const vector<double> &right)
{
    double left_mse = forest_variance(left);
    double right_mse = forest_variance(right);

    return (left.size() * left_mse + right.size() * right_mse) / (left.size() + right.size());
}

void split_data(vector<vector<double>> &data, int feature, double threshold, vector<vector<double>> &left, vector<vector<double>> &right)
{
    for (const auto &row : data)
    {
        if (row[feature] <= threshold)
        {
            left.push_back(row);
        }
        else
        {
            right.push_back(row);
        }
    }
}

TreeNode *tree_build(vector<vector<double>> &data, int depth, int max_depth)
{
    if (data.size() <= 1 || depth >= max_depth)
    {
        TreeNode *node = forest_create_node();
        vector<double> targets;

        for (const auto &row : data)
        {
            targets.push_back(row.back());
        }

        node->value = foprest_mean(targets);
        return node;
    }

    int best_feature = -1;
    double best_threshold = 0.0;
    double best_mse = numeric_limits<double>::max();
    vector<vector<double>> best_left, best_right;

    for (int feature = 0; feature < data[0].size() - 1; ++feature)
    {
        for (const auto &row : data)
        {
            double threshold = row[feature];
            vector<vector<double>> left, right;
            split_data(data, feature, threshold, left, right);

            if (!left.empty() && !right.empty())
            {
                vector<double> left_targets, right_targets;

                for (const auto &l_row : left)
                {
                    left_targets.push_back(l_row.back());
                }

                for (const auto &r_row : right)
                {
                    right_targets.push_back(r_row.back());
                }

                double mse = forest_mse(left_targets, right_targets);

                if (mse < best_mse)
                {
                    best_feature = feature;
                    best_threshold = threshold;
                    best_mse = mse;
                    best_left = left;
                    best_right = right;
                }
            }
        }
    }

    if (best_feature == -1)
    {
        TreeNode *node = forest_create_node();
        vector<double> targets;

        for (const auto &row : data)
        {
            targets.push_back(row.back());
        }

        node->value = foprest_mean(targets);

        return node;
    }

    TreeNode *node = forest_create_node();
    node->feature = best_feature;
    node->threshold = best_threshold;
    node->left = tree_build(best_left, depth + 1, max_depth);
    node->right = tree_build(best_right, depth + 1, max_depth);

    return node;
}

void tree_shuffle(vector<vector<double>> &data)
{
    random_device rd;
    mt19937 g(rd());

    shuffle(data.begin(), data.end(), g);
}

void tree_train_thread(RandomForest *forest, vector<vector<double>> &data, int max_depth, int seed)
{
    tree_shuffle(data);
    forest->trees[seed] = tree_build(data, 0, max_depth);
}

RandomForest *forest_train(vector<vector<double>> &data, int num_trees, int max_depth)
{
    RandomForest *forest = new RandomForest();

    forest->num_trees = num_trees;
    forest->trees.resize(num_trees);

    vector<thread> threads;

    for (int i = 0; i < num_trees; ++i)
    {
        threads.push_back(thread(tree_train_thread, forest, ref(data), max_depth, i));
    }

    for (auto &th : threads)
    {
        th.join();
    }

    return forest;
}

double tree_predict(TreeNode *tree, const vector<double> &instance)
{
    if (tree->feature == -1)
    {
        return tree->value;
    }

    if (instance[tree->feature] <= tree->threshold)
    {
        return tree_predict(tree->left, instance);
    }
    else
    {
        return tree_predict(tree->right, instance);
    }
}

double forest_predict(RandomForest *forest, const vector<double> &instance)
{
    double sum = 0.0;

    for (TreeNode *tree : forest->trees)
    {
        sum += tree_predict(tree, instance);
    }

    return sum / forest->num_trees;
}

void free_forest(RandomForest *forest)
{
    for (TreeNode *tree : forest->trees)
    {
        tree_free(tree);
    }

    delete forest;
}

void tree_save(ofstream &file, TreeNode *node)
{
    if (node == nullptr)
    {
        file << "null\n";
        return;
    }

    file << node->feature << " " << node->threshold << " " << node->value << "\n";

    tree_save(file, node->left);
    tree_save(file, node->right);
}

TreeNode *tree_load(ifstream &file)
{
    string line;

    if (!getline(file, line) || line == "null")
    {
        return nullptr;
    }

    stringstream ss(line);

    int feature;
    double threshold, value;

    ss >> feature >> threshold >> value;

    TreeNode *node = forest_create_node();
    node->feature = feature;
    node->threshold = threshold;
    node->value = value;
    node->left = tree_load(file);
    node->right = tree_load(file);

    return node;
}

void forest_savefile(RandomForest *forest, const string &filename)
{
    ofstream file(filename);

    if (!file)
    {
        cerr << "error: can not save " << filename << "\n";
        exit(1);
    }

    file << forest->num_trees << "\n";

    for (TreeNode *tree : forest->trees)
    {
        tree_save(file, tree);
    }
}

RandomForest *forest_loadfile(const string &filename)
{
    ifstream file(filename);

    if (!file)
    {
        cerr << "error: can not found " << filename << "\n";
        exit(1);
    }

    int num_trees;
    file >> num_trees;

    RandomForest *forest = new RandomForest();
    forest->num_trees = num_trees;

    for (int i = 0; i < num_trees; ++i)
    {
        forest->trees.push_back(tree_load(file));
    }

    return forest;
}

#endif
